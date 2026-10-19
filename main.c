/*
 * main.c - Terminal Othello.
 *
 * Originally written as a course project for ESTR1002 Problem Solving by
 * Programming, The Chinese University of Hong Kong (2022).
 *
 * Usage: run the program, then enter a game mode:
 *   1  Human vs human
 *   2  Human vs computer (colours are assigned at random)
 * Enter moves as a two-digit index: row then column, e.g. 34 = row 3, col 4.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "board.h"
#include "ai_player.h"

enum { MODE_HUMAN_VS_HUMAN = 1, MODE_HUMAN_VS_AI = 2 };

static const char *player_name(int player)
{
    return (player == BLACK) ? "black" : "white";
}

static int opponent_of(int player)
{
    return (player == BLACK) ? WHITE : BLACK;
}

static int is_legal_move(const int board[], int pos, int player)
{
    for (int d = 0; d < NUM_DIRECTIONS; d++)
        if (board_is_valid_direction(board, pos, player, DIRECTIONS[d]))
            return 1;
    return 0;
}

/* Asks the human for moves until a legal one is entered. */
static int read_human_move(const int board[], int player)
{
    int move;
    do
    {
        move = human_choose_move(player, board);
        if (move < 0)
        {
            printf("No more input - exiting.\n");
            exit(EXIT_FAILURE);
        }
        printf("player %s input location:%d\n", player_name(player), move);
    } while (!is_legal_move(board, move, player));
    return move;
}

/*
 * Plays a full game. `ai_colour` is the colour played by the computer,
 * or EMPTY for a two-human game.
 */
static void play_game(int board[], int ai_colour)
{
    int player = BLACK;

    while (board_game_in_progress(board))
    {
        board_print(board);

        if (board_has_legal_move(board, player))
        {
            int move;
            if (player == ai_colour)
            {
                move = ai_choose_move(player, board);
                printf("player %s input location:%d\n", player_name(player), move);
            }
            else
            {
                move = read_human_move(board, player);
            }
            board_apply_move(board, player, move, DIRECTIONS);
        }
        else
        {
            printf("pass from %s\n", player_name(player));
        }

        player = opponent_of(player);
    }
}

int main(void)
{
    int board[BOARD_CELLS] = {0};
    board_init(board);

    int mode = 0;
    if (scanf("%d", &mode) != 1)
        return 1;

    if (mode == MODE_HUMAN_VS_HUMAN)
    {
        play_game(board, EMPTY);
    }
    else if (mode == MODE_HUMAN_VS_AI)
    {
        /* Randomly decide whether the human plays black (moves first). */
        srand((unsigned)time(NULL));
        int human_colour = (rand() % 2) + 1;
        play_game(board, opponent_of(human_colour));
    }

    board_print(board);

    int black_discs = board_count_discs(board, BLACK);
    int white_discs = board_count_discs(board, WHITE);

    if (black_discs > white_discs)
        printf("Black wins!\n");
    else if (black_discs < white_discs)
        printf("White wins!\n");
    else
        printf("Draw\n");

    return 0;
}
