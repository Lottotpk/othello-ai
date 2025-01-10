/**
* ESTR 1002 Problem Solving by Programming
*
* Course Project
*
* I declare that the assignment here submitted is original
* except for source material explicitly acknowledged,
* and that the same or closely related material has not been
* previously submitted for another course.
* I also acknowledge that I am aware of University policy and
* regulations on honesty in academic work, and of the disciplinary
* guidelines and procedures applicable to breaches of such
* policy and regulations, as contained in the website.
*
* University Guideline on Academic Honesty:
* http://www.cuhk.edu.hk/policy/academichonesty/
*
* Student Name : PIPATPAJONG Thapakorn
* Student ID : 1155184202
* Date : 29 Nov 2022
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "aiplayer_1155184202.h"
#include "aiplayer_1155184202_2.h"
#include "aiplayer_1155179480.h"
#include "othello.h"
#include <time.h>

int direction[8] = {-11, -10, -9, -1, 1, 9, 10, 11};

int main()
{
    int board[100] = {0};
    int i;

    initialize_board(board);
    int player = 1, pos;
    int state = check_ending(board);

    int mode;
    scanf("%d", &mode);

    if(mode == 1)
    {
        while(state)
        {
            display_board(board);

            if(can_place(board, player))
            {
                int valid = 0;
                while(!valid)
                {
                    pos = human_player_1155184202(player, board);
                    if(player == 1)
                        printf("player black input location:%d\n", pos);
                    else
                        printf("player white input location:%d\n", pos);

                    for(i = 0; i < 8; i++)
                        if(check_validity(board, pos, player, direction[i]))
                            valid++;
                }
                replace_discs(board, player, pos, direction);
            }

            else
            {
                if(player == 1)
                    printf("pass from black\n");
                else
                    printf("pass from white\n");
            }

            if(player == 1)
                player++;
            else
                player--;
            
            state = check_ending(board);
        }
    }

    else if(mode == 2)
    {
        // random ai to black or white and player otherwise
        srand(time(NULL));
        player = (rand() % 2) + 1;
        // if player is 1 (black), then player start first
        // otherwise bot will start first 
        
        if(player == 1)
        {
            while(state)
            {
                display_board(board);

                if(can_place(board, player))
                {
                    int valid = 0;
                    
                    while(!valid)
                    {
                        if(player == 1)
                        {
                            pos = human_player_1155184202(player, board);
                            printf("player black input location:%d\n", pos);

                            for(i = 0; i < 8; i++)
                                if(check_validity(board, pos, player, direction[i]))
                                    valid++;
                        }

                        else
                        {
                            pos = ai_player_1155184202(player, board);
                            printf("player white input location:%d\n", pos);
                            valid++;
                        }
                    }
                    replace_discs(board, player, pos, direction);
                }

                else
                {
                    if(player == 1)
                        printf("pass from black\n");
                    else
                        printf("pass from white\n");
                }

                if(player == 1)
                    player++;
                else
                    player--;
                
                state = check_ending(board);
            }
        }

        else
        {
            player--;
            while(state)
            {
                display_board(board);

                if(can_place(board, player))
                {
                    int valid = 0;                    
                    while(!valid)
                    {
                        if(player == 1)
                        {
                            pos = ai_player_1155184202(player, board);
                            printf("player black input location:%d\n", pos);
                            valid++;
                        }

                        else
                        {
                            pos = human_player_1155184202(player, board);
                            printf("player white input location:%d\n", pos);

                            for(i = 0; i < 8; i++)
                                if(check_validity(board, pos, player, direction[i]))
                                    valid++;
                        }
                    }
                    replace_discs(board, player, pos, direction);
                }

                else
                {
                    if(player == 1)
                        printf("pass from black\n");
                    else
                        printf("pass from white\n");
                }

                if(player == 1)
                    player++;
                else
                    player--;
                
                state = check_ending(board);
            }   
        }
    }

    else if(mode == 3)
    {
        // random ai to black or white and player otherwise
        srand(time(NULL));
        player = (rand() % 2) + 1;
        // if player is 1 (black), then player start first
        // otherwise bot will start first 
        
        if(player == 1)
        {
            printf("black old, white new\n");
            while(state)
            {
                display_board(board);

                if(can_place(board, player))
                {
                    int valid = 0;
                    
                    while(!valid)
                    {
                        if(player == 1)
                        {
                            pos = ai_player_1155179480(player, board);
                            printf("player black input location:%d\n", pos);
                            valid++;
                        }

                        else
                        {
                            pos = ai_player_1155184202(player, board);
                            printf("player white input location:%d\n", pos);
                            valid++;
                        }
                    }
                    replace_discs(board, player, pos, direction);
                }

                else
                {
                    if(player == 1)
                        printf("pass from black\n");
                    else
                        printf("pass from white\n");
                }

                if(player == 1)
                    player++;
                else
                    player--;
                
                state = check_ending(board);
            }
        }

        else
        {
            printf("black new, white old\n");
            player--;
            while(state)
            {
                display_board(board);

                if(can_place(board, player))
                {
                    int valid = 0;                    
                    while(!valid)
                    {
                        if(player == 1)
                        {
                            pos = ai_player_1155184202(player, board);
                            printf("player black input location:%d\n", pos);
                            valid++;
                        }

                        else
                        {
                            pos = ai_player_1155179480(player, board);
                            printf("player white input location:%d\n", pos);
                            valid++;
                        }
                    }
                    replace_discs(board, player, pos, direction);
                }

                else
                {
                    if(player == 1)
                        printf("pass from black\n");
                    else
                        printf("pass from white\n");
                }

                if(player == 1)
                    player++;
                else
                    player--;
                
                state = check_ending(board);
            }   
        }
    }

    display_board(board);

    int black = count_pieces(board, 1);
    int white = count_pieces(board, 2);

    if(black > white)
        printf("Black wins!\n");
    else if(black < white)
        printf("White wins!\n");
    else
        printf("Draw\n");

    return 0;
}