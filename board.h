/*
 * board.h - Othello board representation and game rules.
 *
 * The board is a 10x10 array with a one-cell border around the 8x8 playing
 * area. Square (row r, column c), with 1 <= r, c <= 8, is stored at index
 * 10*r + c, so playable indices run from 11 to 88. The border cells let the
 * code step in any of the eight directions without bounds checks.
 */
#ifndef OTHELLO_BOARD_H
#define OTHELLO_BOARD_H

#define BOARD_CELLS   100   /* 10 x 10 including the border            */
#define FIRST_SQUARE  11    /* index of row 1, column 1                */
#define LAST_SQUARE   88    /* index of row 8, column 8                */
#define NUM_DIRECTIONS 8

/* Cell contents. BLACK and WHITE also identify the players. */
enum { EMPTY = 0, BLACK = 1, WHITE = 2, BORDER = 3 };

/* Index offsets for the eight neighbouring squares. */
extern const int DIRECTIONS[NUM_DIRECTIONS];

void board_init(int board[]);
int  board_count_discs(const int board[], int player);
void board_print(const int board[]);

/* Returns 1 if placing at pos flips at least one disc in this direction. */
int  board_is_valid_direction(const int board[], int pos, int player, int direction);
/* Returns 1 if the player has at least one legal move. */
int  board_has_legal_move(const int board[], int player);
/* Returns 1 while either player can still move. */
int  board_game_in_progress(const int board[]);
/* Places a disc for the player at pos and flips the captured discs. */
void board_apply_move(int board[], int player, int pos, const int directions[]);

/* Reads a move (two-digit index such as 34) from standard input.
   Returns -1 if no number could be read. */
int  human_choose_move(int player, const int *board);

#endif /* OTHELLO_BOARD_H */
