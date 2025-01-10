#include <stdio.h>
#include "aiplayer_1155184202.h"

int all_direction[8] = {-11, -10, -9, -1, 1, 9, 10, 11};
int corner[4] = {11, 18, 81, 88};
int x_sq[4] = {22, 27, 72, 77};
int c_sq[8] = {12, 17, 21, 28, 71, 78, 82, 87};
int a_sq[8] = {13, 16, 31, 38, 61, 68, 83, 86};
int b_sq[8] = {14, 15, 41, 48, 51, 58, 84, 85};
int sq4x4[16] = {33, 34, 35, 36, 43, 44, 45, 46, 53, 54, 55, 56, 63, 64, 65, 66};

int ai_player_1155184202(int player, const int *board)
{
    int x[100] = {0};
    printf("%d ", x);
    for(int i = 0; i < 100; i++)
        x[i] = board[i];

    printf("%d", x);
    return 0;
}

int main()
{
    int player = 1;
    int board[100] = {0};
    int res = ai_player_1155184202(player, board);
    int x[100] = {0};
    printf(" %d ", *x);
    return 0;
}