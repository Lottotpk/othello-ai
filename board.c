/*
 * board.c - Othello board representation, move validation and display.
 */
#include <stdio.h>
#include <string.h>
#include "board.h"

const int DIRECTIONS[NUM_DIRECTIONS] = {-11, -10, -9, -1, 1, 9, 10, 11};

void board_init(int board[])
{
    int i;

    /* Top border row. */
    for (i = 0; i < 10; i++)
        board[i] = BORDER;

    /* Left and right border columns. */
    for (; i < 90; i += 10)
    {
        board[i] = BORDER;
        board[i + 9] = BORDER;
    }

    /* Bottom border row. */
    for (; i < BOARD_CELLS; i++)
        board[i] = BORDER;

    /* Standard starting position. */
    board[44] = WHITE;
    board[45] = BLACK;
    board[54] = BLACK;
    board[55] = WHITE;
}

int board_count_discs(const int board[], int player)
{
    int count = 0;

    for (int i = FIRST_SQUARE; i <= LAST_SQUARE; i++)
        if (board[i] == player)
            count++;

    return count;
}

void board_print(const int board[])
{
    int black_discs = board_count_discs(board, BLACK);
    int white_discs = board_count_discs(board, WHITE);

    printf("   1 2 3 4 5 6 7 8 [#=%d O=%d]\n", black_discs, white_discs);
    for (int row = 1; row <= 8; row++)
    {
        printf("%d  ", row);
        for (int col = 1; col <= 8; col++)
        {
            int cell = board[10 * row + col];
            if (cell == EMPTY)
                printf(". ");
            else if (cell == BLACK)
                printf("# ");
            else
                printf("O ");
        }
        printf("\n");
    }
}

int human_choose_move(int player, const int *board)
{
    (void)player;
    (void)board;

    int move = 0;
    if (scanf("%d", &move) != 1)
        return -1;   /* end of input or non-numeric input */
    return move;
}

int board_is_valid_direction(const int board[], int pos, int player, int direction)
{
    if (pos < FIRST_SQUARE || pos > LAST_SQUARE || board[pos] != EMPTY)
        return 0;

    int opponent = (player == BLACK) ? WHITE : BLACK;

    /* The neighbouring square must hold an opponent disc... */
    if (board[pos + direction] != opponent)
        return 0;

    /* ...followed by more opponent discs and then one of the player's own. */
    int cursor = pos + 2 * direction;
    while (cursor >= FIRST_SQUARE && cursor <= LAST_SQUARE && board[cursor] != EMPTY
           && cursor % 10 != 9 && cursor % 10 != 0)
    {
        if (board[cursor] == player)
            return 1;
        cursor += direction;
    }

    return 0;
}

int board_has_legal_move(const int board[], int player)
{
    for (int pos = FIRST_SQUARE; pos <= LAST_SQUARE; pos++)
        for (int d = 0; d < NUM_DIRECTIONS; d++)
            if (board_is_valid_direction(board, pos, player, DIRECTIONS[d]))
                return 1;

    return 0;
}

int board_game_in_progress(const int board[])
{
    return board_has_legal_move(board, BLACK) || board_has_legal_move(board, WHITE);
}

void board_apply_move(int board[], int player, int pos, const int directions[])
{
    int before[BOARD_CELLS];
    memcpy(before, board, sizeof before);

    board[pos] = player;

    for (int d = 0; d < NUM_DIRECTIONS; d++)
    {
        if (!board_is_valid_direction(before, pos, player, directions[d]))
            continue;

        int cursor = pos + directions[d];
        while (cursor >= FIRST_SQUARE && cursor <= LAST_SQUARE
               && before[cursor] != player && before[cursor] != EMPTY)
        {
            board[cursor] = player;
            cursor += directions[d];
        }
    }
}
