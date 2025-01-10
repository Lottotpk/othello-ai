#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "aiplayer_1155184202.h"
#include "othello.h"

int directions[8] = {-11, -10, -9, -1, 1, 9, 10, 11};

void initialize_board(int board[])
{
    int i;

    for(i = 0; i < 10; i++)
        board[i] = 3;
    
    for(; i < 90; i += 10)
    {
        board[i] = 3;
        board[i+9] = 3;
    }
    
    for(; i < 100; i++)
        board[i] = 3;

    board[44] = 2;
    board[45] = 1;
    board[54] = 1;
    board[55] = 2;
}

int count_pieces(int board[], int player)
{
    int count = 0;

    for(int i = 11; i < 89; i++)
        if(board[i] == player)
            count++;

    return count++;
}

void display_board(int board[])
{
    int i, j;

    int black = count_pieces(board, 1);
    int white = count_pieces(board, 2);

    printf("   1 2 3 4 5 6 7 8 [#=%d O=%d]\n", black, white);
    for(i = 1; i < 9; i++)
    {
        printf("%d  ", i);
        for(j = 1; j < 9; j++)
        {
            if(board[10*i+j] == 0)
                printf(". ");
            else if(board[10*i+j] == 1)
                printf("# ");
            else
                printf("O ");
        }
        printf("\n");
    }
}

int human_player_1155184202(int player, const int *board)
{
    int pos;
    scanf("%d", &pos);
    return pos;
}

int check_validity(int board[], int pos, int player, int direction)
{
    if(board[pos] == 0 && pos > 10 && pos < 89)
    {
        int opponent;
        if(player == 1)
            opponent = 2;
        else
            opponent = 1;

        int moving_pos = pos;

        if(board[pos+direction] == opponent)
        {
            moving_pos += 2*direction;
            while((moving_pos > 10) && (moving_pos < 89) && (board[moving_pos] != 0) && (moving_pos % 10 != 9) && (moving_pos % 10 != 0))
            {
                if(board[moving_pos] == player)
                    return 1;
                moving_pos += direction;
            }
        }
        
        else
            return 0;
    }

    else
        return 0;
    
    return 0;
}

int can_place(int board[], int player)
{
    for(int i = 11; i < 89; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(check_validity(board, i, player, directions[j]))
                return 1;
        }
    }

    return 0;
}

int check_ending(int board[])
{
    if(can_place(board, 1) || can_place(board, 2))
        return 1;

    else
        return 0;

    for(int i = 11; i < 89; i++)
    {
        if(board[i] == 0)
            return 1;
    }

    return 0;
}

void replace_discs(int board[], int player, int pos, int direction[])
{
    int tmp_board[100];
    memcpy(tmp_board, board, 400);

    board[pos] = player;

    for(int i = 0; i < 8; i++)
    {
        if(check_validity(tmp_board, pos, player, direction[i]))
        {
            int moving_pos = pos + direction[i];

            while(moving_pos > 10 && moving_pos < 89)
            {
                if(tmp_board[moving_pos] != player && tmp_board[moving_pos] != 0)
                {
                    board[moving_pos] = player;
                    moving_pos += direction[i];
                }

                else
                    break;
            }
        }
    }
}