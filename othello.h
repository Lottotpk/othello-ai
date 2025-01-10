#ifndef _OTHELLO_H
#define _OTHELLO_H

void initialize_board(int board[]);
int count_pieces(int board[], int player);
void display_board(int board[]);
int human_player_1155184202(int player, const int *board);
int check_validity(int board[], int pos, int player, int direction);
int can_place(int board[], int player);
int check_ending(int board[]);
void replace_discs(int board[], int player, int pos, int direction[]);

#endif