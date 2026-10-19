/*
 * ai_player.c - Computer opponent for Othello.
 *
 * Strategy
 *   1. Take a corner whenever one is available.
 *   2. Otherwise run a depth-limited minimax search with alpha-beta pruning.
 *      - Opening / middlegame (fewer than OPENING_DISC_LIMIT discs on the
 *        board): minimise our own disc count. Holding few discs early keeps
 *        our mobility high and the opponent's low (the "evaporation"
 *        strategy). On our turn, A- and B-squares on the edges are searched
 *        first; if none is legal, every legal move is searched.
 *      - Late game: maximise our own disc count.
 *   3. In both searches the AI avoids X-squares (diagonally next to a
 *      corner) and only plays a C-square (orthogonally next to a corner)
 *      when the resulting edge holds no opponent disc and at least
 *      MIN_OWN_EDGE_DISCS of our own.
 *
 * Moves are applied in place and undone afterwards, using the list of
 * flipped discs, so the search never copies the board.
 */
#include <string.h>
#include "board.h"
#include "ai_player.h"

#define SEARCH_DEPTH        8     /* plies searched from the root               */
#define OPENING_DISC_LIMIT  30    /* below this many discs, use the opening plan */
#define MIN_OWN_EDGE_DISCS  5     /* needed on an edge before taking a C-square  */
#define MAX_CANDIDATES      512   /* (square, direction) pairs; 60 x 8 is the cap */
#define MAX_FLIPS           32    /* a single move flips at most 18 discs        */
#define INF                 999999

/* Scores given to leaf positions that end the line early.
   Disc counts lie in 0..64, so 65 and -1 are outside the normal range. */
#define BEST_FOR_MINIMISER    0
#define WORST_FOR_MINIMISER   65
#define BEST_FOR_MAXIMISER    65
#define WORST_FOR_MAXIMISER   (-1)

static const int CORNERS[4] = {11, 18, 81, 88};

/* Edge squares two and three steps from a corner (A) and the centre of
   each edge (B). These are generally safe edge moves. */
static const int A_SQUARES[8] = {13, 16, 31, 38, 61, 68, 83, 86};
static const int B_SQUARES[8] = {14, 15, 41, 48, 51, 58, 84, 85};

/* ---------------------------------------------------------------------- */
/* Square classification                                                  */
/* ---------------------------------------------------------------------- */

static int opponent_of(int player)
{
    return (player == BLACK) ? WHITE : BLACK;
}

static int is_corner(int pos)
{
    return pos == 11 || pos == 18 || pos == 81 || pos == 88;
}

/* X-squares sit diagonally next to a corner. */
static int is_x_square(int pos)
{
    return pos == 22 || pos == 27 || pos == 72 || pos == 77;
}

/* C-squares sit on an edge, orthogonally next to a corner. */
static int is_c_square(int pos)
{
    return pos == 12 || pos == 17 || pos == 21 || pos == 28
        || pos == 71 || pos == 78 || pos == 82 || pos == 87;
}

/* Finds the edge a C-square belongs to: its first square and the step
   between consecutive squares along it. */
static void edge_of_c_square(int pos, int *edge_start, int *edge_step)
{
    if (pos == 12 || pos == 17)      { *edge_start = 11; *edge_step = 1;  } /* top    */
    else if (pos == 21 || pos == 71) { *edge_start = 11; *edge_step = 10; } /* left   */
    else if (pos == 28 || pos == 78) { *edge_start = 18; *edge_step = 10; } /* right  */
    else                             { *edge_start = 81; *edge_step = 1;  } /* bottom */
}

/* ---------------------------------------------------------------------- */
/* Move generation                                                        */
/* ---------------------------------------------------------------------- */

/* Returns the first corner the player can legally take, or 0 if none. */
static int first_legal_corner(const int board[], int player)
{
    for (int i = 0; i < 4; i++)
        for (int d = 0; d < NUM_DIRECTIONS; d++)
            if (board_is_valid_direction(board, CORNERS[i], player, DIRECTIONS[d]))
                return CORNERS[i];
    return 0;
}

/*
 * Appends the legal moves found among `squares` to `moves`, starting at
 * index n, and returns the new length. A square is appended once for each
 * direction in which it captures, so a move may appear more than once;
 * the search relies on this ordering and it is kept deliberately.
 */
static int append_legal_moves(const int board[], int player, const int squares[],
                              int num_squares, int moves[], int n)
{
    for (int i = 0; i < num_squares; i++)
        for (int d = 0; d < NUM_DIRECTIONS; d++)
            if (board_is_valid_direction(board, squares[i], player, DIRECTIONS[d]))
                moves[n++] = squares[i];
    return n;
}

/* Lists every legal move (with the same per-direction repetition). */
static int list_all_legal_moves(const int board[], int player, int moves[])
{
    int n = 0;
    for (int pos = FIRST_SQUARE; pos <= LAST_SQUARE; pos++)
        if (pos % 10 != 0 && pos % 10 != 9)
            for (int d = 0; d < NUM_DIRECTIONS; d++)
                if (board_is_valid_direction(board, pos, player, DIRECTIONS[d]))
                    moves[n++] = pos;
    return n;
}

/* ---------------------------------------------------------------------- */
/* Make / unmake                                                          */
/* ---------------------------------------------------------------------- */

/* Plays `pos` for `player` and records the flipped squares.
   Returns the number of discs flipped. */
static int make_move(int board[], int player, int pos, int flipped[])
{
    int before[BOARD_CELLS];
    memcpy(before, board, sizeof before);

    board[pos] = player;
    int num_flipped = 0;

    for (int d = 0; d < NUM_DIRECTIONS; d++)
    {
        if (!board_is_valid_direction(before, pos, player, DIRECTIONS[d]))
            continue;

        int cursor = pos + DIRECTIONS[d];
        while (cursor >= FIRST_SQUARE && cursor <= LAST_SQUARE
               && before[cursor] != player && before[cursor] != EMPTY)
        {
            board[cursor] = player;
            flipped[num_flipped++] = cursor;
            cursor += DIRECTIONS[d];
        }
    }
    return num_flipped;
}

/* Reverses make_move: empties `pos` and gives the flipped discs back. */
static void unmake_move(int board[], int pos, const int flipped[], int num_flipped,
                        int previous_owner)
{
    board[pos] = EMPTY;
    for (int i = 0; i < num_flipped; i++)
        board[flipped[i]] = previous_owner;
}

/* ---------------------------------------------------------------------- */
/* Positional rules                                                       */
/* ---------------------------------------------------------------------- */

/* A C-square move is safe if, after playing it, its edge holds no opponent
   disc and at least MIN_OWN_EDGE_DISCS of the player's own discs. */
static int c_square_move_is_safe(int board[], int player, int pos)
{
    int flipped[MAX_FLIPS];
    int num_flipped = make_move(board, player, pos, flipped);

    int edge_start, edge_step;
    edge_of_c_square(pos, &edge_start, &edge_step);

    int own_discs = 0, opponent_on_edge = 0;
    for (int i = 0, sq = edge_start; i < 8; i++, sq += edge_step)
    {
        if (board[sq] != player && board[sq] != EMPTY)
        {
            opponent_on_edge = 1;
            break;
        }
        if (board[sq] == player)
            own_discs++;
    }

    unmake_move(board, pos, flipped, num_flipped, opponent_of(player));
    return !opponent_on_edge && own_discs >= MIN_OWN_EDGE_DISCS;
}

/*
 * Decides whether the AI should skip a candidate move:
 *   - X-squares are skipped whenever any other move exists.
 *   - C-squares are skipped when another move exists and the edge is unsafe.
 */
static int should_skip_move(int board[], int player, int pos,
                            int has_non_x_move, int has_non_c_move)
{
    if (is_x_square(pos))
        return has_non_x_move;
    if (is_c_square(pos) && has_non_c_move)
        return !c_square_move_is_safe(board, player, pos);
    return 0;
}

static void scan_candidates(const int moves[], int n, int *has_non_x, int *has_non_c)
{
    *has_non_x = 0;
    *has_non_c = 0;
    for (int i = 0; i < n; i++)
    {
        if (!is_x_square(moves[i])) *has_non_x = 1;
        if (!is_c_square(moves[i])) *has_non_c = 1;
    }
}

/* ---------------------------------------------------------------------- */
/* Search                                                                 */
/* ---------------------------------------------------------------------- */

/*
 * Opening / middlegame search: the AI minimises its own disc count and the
 * opponent maximises it. At the root (depth == SEARCH_DEPTH) the chosen
 * move is returned instead of its score.
 */
static int search_min_discs(int board[], int depth, int alpha, int beta,
                            int player, int ai_colour)
{
    if (depth == 0)
        return board_count_discs(board, ai_colour);

    int opponent = opponent_of(player);
    int moves[MAX_CANDIDATES];
    int flipped[MAX_FLIPS];

    if (player != ai_colour)   /* opponent's turn: maximise our disc count */
    {
        int n = list_all_legal_moves(board, player, moves);
        int best_score = -INF;

        for (int i = 0; i < n; i++)
        {
            int num_flipped = make_move(board, player, moves[i], flipped);
            int score = is_corner(moves[i])
                      ? WORST_FOR_MINIMISER
                      : search_min_discs(board, depth - 1, alpha, beta, opponent, ai_colour);
            unmake_move(board, moves[i], flipped, num_flipped, opponent);

            if (score > best_score) best_score = score;
            if (score > alpha)      alpha = score;
            if (alpha >= beta)      break;
        }
        return best_score;
    }

    /* AI's turn: try A- and B-squares first, otherwise every legal move. */
    int n = append_legal_moves(board, player, A_SQUARES, 8, moves, 0);
    n = append_legal_moves(board, player, B_SQUARES, 8, moves, n);
    if (n == 0)
        n = list_all_legal_moves(board, player, moves);

    int has_non_x, has_non_c;
    scan_candidates(moves, n, &has_non_x, &has_non_c);

    int best_move = (n > 0) ? moves[0] : 0;
    int best_score = INF;

    for (int i = 0; i < n; i++)
    {
        if (should_skip_move(board, player, moves[i], has_non_x, has_non_c))
            continue;

        int num_flipped = make_move(board, player, moves[i], flipped);
        int score = is_corner(moves[i])
                  ? BEST_FOR_MINIMISER
                  : search_min_discs(board, depth - 1, alpha, beta, opponent, ai_colour);
        unmake_move(board, moves[i], flipped, num_flipped, opponent);

        if (score < best_score)
        {
            best_score = score;
            best_move = moves[i];
        }
        if (score < beta)  beta = score;
        if (alpha >= beta) break;
    }

    return (depth == SEARCH_DEPTH) ? best_move : best_score;
}

/*
 * Late-game search: the AI maximises its own disc count and the opponent
 * minimises it. At the root (depth == SEARCH_DEPTH) the chosen move is
 * returned instead of its score.
 */
static int search_max_discs(int board[], int depth, int alpha, int beta,
                            int player, int ai_colour)
{
    if (depth == 0)
        return board_count_discs(board, ai_colour);

    int opponent = opponent_of(player);
    int moves[MAX_CANDIDATES];
    int flipped[MAX_FLIPS];
    int n = list_all_legal_moves(board, player, moves);

    if (player != ai_colour)   /* opponent's turn: minimise our disc count */
    {
        int best_score = INF;

        for (int i = 0; i < n; i++)
        {
            int num_flipped = make_move(board, player, moves[i], flipped);
            int score = is_corner(moves[i])
                      ? WORST_FOR_MAXIMISER
                      : search_max_discs(board, depth - 1, alpha, beta, opponent, ai_colour);
            unmake_move(board, moves[i], flipped, num_flipped, opponent);

            if (score < best_score) best_score = score;
            if (score < beta)       beta = score;
            if (alpha >= beta)      break;
        }
        return best_score;
    }

    /* AI's turn. */
    int has_non_x, has_non_c;
    scan_candidates(moves, n, &has_non_x, &has_non_c);

    int best_move = (n > 0) ? moves[0] : 0;
    int best_score = -INF;

    for (int i = 0; i < n; i++)
    {
        if (should_skip_move(board, player, moves[i], has_non_x, has_non_c))
            continue;

        int num_flipped = make_move(board, player, moves[i], flipped);
        int score = is_corner(moves[i])
                  ? BEST_FOR_MAXIMISER
                  : search_max_discs(board, depth - 1, alpha, beta, opponent, ai_colour);
        unmake_move(board, moves[i], flipped, num_flipped, opponent);

        if (score > best_score)
        {
            best_score = score;
            best_move = moves[i];
        }
        if (score > alpha) alpha = score;
        if (alpha >= beta) break;
    }

    return (depth == SEARCH_DEPTH) ? best_move : best_score;
}

/* ---------------------------------------------------------------------- */
/* Public entry point                                                     */
/* ---------------------------------------------------------------------- */

int ai_choose_move(int player, const int *board)
{
    int work[BOARD_CELLS];
    memcpy(work, board, sizeof work);

    int corner = first_legal_corner(work, player);
    if (corner)
        return corner;

    int discs_on_board = board_count_discs(work, BLACK) + board_count_discs(work, WHITE);

    if (discs_on_board < OPENING_DISC_LIMIT)
        return search_min_discs(work, SEARCH_DEPTH, -INF, INF, player, player);
    else
        return search_max_discs(work, SEARCH_DEPTH, -INF, INF, player, player);
}
