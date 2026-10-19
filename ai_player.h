/*
 * ai_player.h - Computer opponent for Othello.
 */
#ifndef OTHELLO_AI_PLAYER_H
#define OTHELLO_AI_PLAYER_H

/*
 * Returns the board index (11-88) of the move chosen for `player`
 * (BLACK or WHITE) on the given board. Call only when the player has
 * at least one legal move.
 */
int ai_choose_move(int player, const int *board);

#endif /* OTHELLO_AI_PLAYER_H */
