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
* Date : 22 Nov 2022
*/

#include <stdio.h>
#include <string.h>
#include "aiplayer_1155184202_2.h"

int all_direction2[8] = {-11, -10, -9, -1, 1, 9, 10, 11};
int corner2[4] = {11, 18, 81, 88};
int x_sq2[4] = {22, 27, 72, 77};
int c_sq2[8] = {12, 17, 21, 28, 71, 78, 82, 87};
int a_sq2[8] = {13, 16, 31, 38, 61, 68, 83, 86};
int b_sq2[8] = {14, 15, 41, 48, 51, 58, 84, 85};
int sq_4x42[16] = {33, 34, 35, 36, 43, 44, 45, 46, 53, 54, 55, 56, 63, 64, 65, 66};

int check_validity_1155184202_2(int dup_board[], int pos, int player, int direction)
{
    if(dup_board[pos] == 0 && pos > 10 && pos < 89)
    {
        int opponent;
        if(player == 1)
            opponent = 2;
        else
            opponent = 1;

        int moving_pos = pos;

        if(dup_board[pos+direction] == opponent)
        {
            moving_pos += 2*direction;
            while((moving_pos > 10) && (moving_pos < 89) && (dup_board[moving_pos] != 0) && (moving_pos % 10 != 9) && (moving_pos % 10 != 0))
            {
                if(dup_board[moving_pos] == player)
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

int check_corners_1155184202_2(int player, int dup_board[])
{
    // can speed up    
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 8; j++)
            if(check_validity_1155184202_2(dup_board, corner2[i], player, all_direction2[j]))
                return corner2[i];
    return 0;
}

int can_a_1155184202_2(int player, int dup_board[])
{
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            if(check_validity_1155184202_2(dup_board, a_sq2[i], player, all_direction2[j]))
                return a_sq2[i];
    
    return 0;
}

int can_b_1155184202_2(int player, int dup_board[])
{
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            if(check_validity_1155184202_2(dup_board, b_sq2[i], player, all_direction2[j]))
                return b_sq2[i];
    
    return 0;
}

int can_x_1155184202_2(int player, int dup_board[])
{
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            if(check_validity_1155184202_2(dup_board, x_sq2[i], player, all_direction2[j]))
                return x_sq2[i];
    
    return 0;
}

int can4x4_1155184202_2(int player, int dup_board[])
{
    for(int i = 0; i < 16; i++)
        for(int j = 0; j < 8; j++)
            if(check_validity_1155184202_2(dup_board, sq_4x42[i], player, all_direction2[j]))
                return 1;
    return 0;
}

void fill4x4_1155184202_2(int player, int dup_board[], int slot[])
{
    int n = 0 ;
    for(int i = 0; i < 16; i++)
        for(int j = 0; j < 8; j++)
            if(check_validity_1155184202_2(dup_board, sq_4x42[i], player, all_direction2[j]))
            {
                slot[n] = sq_4x42[i];
                n++;
            }
}

void available_slot_1155184202_2(int player, int dup_board[], int slot[])
{
    int n = 0;
    for(int i = 11; i < 89; i++)
        if((i % 10 != 0) && (i % 10 != 9))
            for(int j = 0; j < 8; j++)
                if(check_validity_1155184202_2(dup_board, i, player, all_direction2[j]))
                {
                    slot[n] = i;
                    n++;
                }
}

void replace_discs_1155184202_2(int dup_board[], int player, int pos, int direction[])
{
    int tmp_board[100];
    memcpy(tmp_board, dup_board, 400);

    dup_board[pos] = player;

    for(int i = 0; i < 8; i++)
    {
        if(check_validity_1155184202_2(tmp_board, pos, player, direction[i]))
        {
            int moving_pos = pos + direction[i];

            while(moving_pos > 10 && moving_pos < 89)
            {
                if(tmp_board[moving_pos] != player && tmp_board[moving_pos] != 0)
                {
                    dup_board[moving_pos] = player;
                    moving_pos += direction[i];
                }

                else
                    break;
            }
        }
    }
}

int count_pieces_1155184202_2(int dup_board[], int player)
{
    int count = 0;

    for(int i = 11; i < 89; i++)
        if(dup_board[i] == player)
            count++;

    return count++;
}

int simulate_move_min_1155184202_2(int player, int dup_board[], int recursion_times, int actual_player)
{
    if(recursion_times > 0)
    {
        int dummy_board[100];
        memcpy(dummy_board, dup_board, 400);

        int slot[64] = {0};

        if(can4x4_1155184202_2(player, dup_board) && (player == actual_player))
            fill4x4_1155184202_2(player, dup_board, slot);
        else
            available_slot_1155184202_2(player, dup_board, slot);

        int res = 65;
        int pos = slot[0];
        int count_x = 0;

        for(int i = 0; slot[i] != 0; i++)
            if(slot[i] != 22 || slot[i] != 27 || slot[i] != 72 || slot[i] != 77)
                count_x++;

        if(slot[0] != 0)
        {
            for(int i = 0; slot[i] != 0; i++)
            {
                //printf("available slot = %d\n", slot[i]);
                if((slot[i] == 22 || slot[i] == 27 || slot[i] == 72 || slot[i] == 77) && (count_x > 0) && (player == actual_player))
                    continue;
                
                replace_discs_1155184202_2(dummy_board, player, slot[i], all_direction2);
                
                if(player == 1)
                    player++;
                else
                    player--;

                int res_cmp;
                if((slot[i] == 11 || slot[i] == 18 || slot[i] == 81 || slot[i] == 88) && (player == actual_player))
                    res_cmp = 0;
                else if((slot[i] == 11 || slot[i] == 18 || slot[i] == 81 || slot[i] == 88) && (player != actual_player))
                    res_cmp = 65;
                else
                    res_cmp = simulate_move_min_1155184202_2(player, dummy_board, recursion_times-1, actual_player);
                
                //printf("res = %d, res_cmp = %d\n", res, res_cmp);
                if(res > res_cmp)
                    {
                        res = res_cmp;
                        pos = slot[i];
                    }
            }
            return pos;
        }

        else
            return count_pieces_1155184202_2(dup_board, player);
    }

    else
        return count_pieces_1155184202_2(dup_board, player);
}

int simulate_move_max_1155184202_2(int player, int dup_board[], int recursion_times, int actual_player)
{
    if(recursion_times > 0)
    {
        int dummy_board[100];
        memcpy(dummy_board, dup_board, 400);

        int slot[64] = {0};
        available_slot_1155184202_2(player, dup_board, slot);

        int res = 0;
        int pos = slot[0];
        int count_x = 0;

        for(int i = 0; slot[i] != 0; i++)
            if(slot[i] != 22 || slot[i] != 27 || slot[i] != 72 || slot[i] != 77)
                count_x++;

        if(slot[0] != 0)
        {
            for(int i = 0; slot[i] != 0; i++)
            {
                if((slot[i] == 22 || slot[i] == 27 || slot[i] == 72 || slot[i] == 77) && (count_x > 0) && (player == actual_player))
                    continue;
                //printf("available slot = %d\n", slot[i]);
                replace_discs_1155184202_2(dummy_board, player, slot[i], all_direction2);
                
                if(player == 1)
                    player++;
                else
                    player--;
                
                int res_cmp;
                if((slot[i] == 11 || slot[i] == 18 || slot[i] == 81 || slot[i] == 88) && (player == actual_player))
                    res_cmp = 65;
                else if((slot[i] == 11 || slot[i] == 18 || slot[i] == 81 || slot[i] == 88) && (player != actual_player))
                    res_cmp = 0;
                else
                    res_cmp = simulate_move_max_1155184202_2(player, dummy_board, recursion_times-1, actual_player);
                
                //printf("res = %d, res_cmp = %d\n", res, res_cmp);
                if(res < res_cmp)
                    {
                        res = res_cmp;
                        pos = slot[i];
                    }
            }
            return pos;
        }

        else
            return count_pieces_1155184202_2(dup_board, player);
    }

    else
        return count_pieces_1155184202_2(dup_board, player);
}

int ai_player_1155184202_2(int player, const int *board)
{
    int opponent, count = 0;
    if(player == 1)
        opponent == 2;
    else
        opponent == 1;

    int dup_board[100];
    for(int i = 0; i < 100; i++)
        dup_board[i] = board[i];

    
    int cor = check_corners_1155184202_2(player, dup_board);
    int c_a = can_a_1155184202_2(player, dup_board);
    int c_b = can_b_1155184202_2(player, dup_board);

    if(cor)
        return cor;
    
    else if(c_a)
        return c_a;
    
    else if(c_b)
        return c_b;

    else if(can4x4_1155184202_2(player, dup_board))
        return simulate_move_min_1155184202_2(player, dup_board, 6, player);
    
    else
    {
        for(int i = 11; i < 89; i++)
            if(dup_board[i] == 0)
                count++;
        
        if(count < 6)
            return simulate_move_max_1155184202_2(player, dup_board, count, player);
        else
            return simulate_move_max_1155184202_2(player, dup_board, 6, player);
    }
}