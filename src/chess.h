#ifndef WHETHER_BE_CITED_OR_NOT_CHESS_H
#define WHETHER_BE_CITED_OR_NOT_CHESS_H

#include <stdio.h>
#include <winerror.h>
#include <stdlib.h>
#include <winsock2.h>
#include <winsock.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <string.h>
#include <math.h>

int max_(int a, int b)
{
    return (a > b) ? a : b;
}
int min_(int a, int b)
{
    return (a < b) ? a : b;
}
void Quit()
{
    WSACleanup(); // 停止对ws2_32的调用
    exit(1);
};
int ChangeCharacterToNumber(char s[], int len)
{
    int answer = 0;
    for (int i = 0; i < len; i++)
        answer = answer * 10 + (s[i] - '0');
    return answer;
} // 字符转数字
int transfer_x(char *s)
{
    if (*s == 'u')
        return 10;
    if (*s == 'y')
        return 11;
    if (*s == 'n')
        return 12;
    int N = strlen(s);
    int number = ChangeCharacterToNumber(s, N);
    return ((number - 84) / 78);
}
int transfer_y(char *s)
{
    if (*s == 'u')
        return 10;
    if (*s == 'y')
        return 11; // 同意
    if (*s == 'n')
        return 12; // 不同意
    int N = strlen(s);
    int number = ChangeCharacterToNumber(s, N);
    return ((number - 47) / 78);
}
int Judge(int old_x, int old_y, int new_x, int new_y, int chessboard[][9], int kind) // 违法点击事件不会传入，不用考虑数组越界
{
    if (new_x == -3) // 同意
        return 3;
    if (new_x == -4) // 不同意
        return 4;
    if (new_x == 11 || new_x == 12 || old_x == 11 || old_x == 12)
        return 0;
    if ((old_x == 10 || old_x == -2) && (chessboard[new_y][new_x] % 12 == 11 || chessboard[new_y][new_x] % 12 == 0)) // 士和将没有给技能
        return 0;
    if (new_x == 10 || new_x == -2) // 防止先点击棋子再点击护摩触发未定义行为
        return 0;
    if ((kind == 0 || kind == 3) && (old_x == 10 || old_x == -2) && chessboard[new_y][new_x] > 12) // judge红棋
        return 0;
    if ((kind == 1 || kind == 2) && (old_x == 10 || old_x == -2) && chessboard[new_y][new_x] < 12) // judge黑棋
        return 0;
    if (old_x == 10 && new_x != 10)
        return 2;
    if (old_x == -2 && new_x != -2)
        return 2;
    if ((kind == 0 || kind == 3) && chessboard[old_y][old_x] > 12) // judge红棋
        return 0;
    if ((kind == 1 || kind == 2) && chessboard[old_y][old_x] < 12) // judge黑棋
        return 0;                                                  // 妄图控制对方的棋子是不行的*/
    int vector = 0;                                                // 方向判定 马：1向左 2向右 3向下 4向上 象：1左上 2右上 3左下 4右下
    if ((chessboard[old_y][old_x] - 12.5) * (chessboard[new_y][new_x] - 12.5) > 0 && chessboard[new_y][new_x])
        return 0; // 不能吃自己的棋子
    switch (chessboard[old_y][old_x])
    {
    case 1: // 车
    case 13:
    case 2:
    case 14:
        if (old_x == new_x) // column相同
        {
            for (int i = min_(old_y, new_y) + 1; i < max_(old_y, new_y); i++)
                if (chessboard[i][old_x])
                    return 0;
            return 1;
        }
        if (old_y == new_y) // row相同
        {
            for (int i = min_(old_x, new_x) + 1; i < max_(old_x, new_x); i++)
                if (chessboard[old_y][i])
                    return 0;
            return 1;
        }
        if (fabs(old_x - new_x) == fabs(old_y - new_y) && old_x != new_x && (chessboard[old_y][old_x] == 2 || chessboard[old_y][old_x] == 14))
        {
            int min_x = min_(old_x, new_x), max_x = max_(old_x, new_x);
            int min_y = min_(old_y, new_y), max_y = max_(old_y, new_y);
            if ((old_x - new_x) * (old_y - new_y) > 0) // 左上  右下
            {
                for (int x = min_x + 1, y = min_y + 1; x < max_x, y < max_y; x++, y++)
                    if (chessboard[y][x])
                        return 0;
                return 1;
            }
            if ((old_x - new_x) * (old_y - new_y) < 0) // 左下 右上
            {
                for (int x = min_x + 1, y = max_y - 1; x<max_x, y> min_y; x++, y--)
                    if (chessboard[y][x])
                        return 0;
                return 1;
            }
        }
        break;
    case 3: // 马
    case 15:
    case 4:
    case 16:
        if ((old_x - new_x) * (old_y - new_y) != 0 && fabs(old_x - new_x) + fabs(old_y - new_y) == 3)
        {
            if (chessboard[old_y][old_x] == 4 || chessboard[old_y][old_x] == 16)
                return 1;
            if (old_x - new_x == 2) // 向左
                vector = 1;
            if (new_x - old_x == 2) // 向右
                vector = 2;
            if (new_y - old_y == 2) // 向下
                vector = 3;
            if (old_y - new_y == 2) // 向上
                vector = 4;
            switch (vector)
            {
            case 1:
                if (!chessboard[old_y][old_x - 1])
                    return 1;
                break;
            case 2:
                if (!chessboard[old_y][old_x + 1])
                    return 1;
                break;
            case 3:
                if (!chessboard[old_y + 1][old_x])
                    return 1;
                break;
            case 4:
                if (!chessboard[old_y - 1][old_x])
                    return 1;
                break;
            default:
                break;
            }
        }
        break;
    case 5: // 相
    case 17:
    case 6:
    case 18:
        if (fabs(old_x - new_x) == 2 && fabs(old_y - new_y) == 2 && (old_y - 4.5) * (new_y - 4.5) > 0)
        {
            if (chessboard[old_y][old_x] == 6 || chessboard[old_y][old_x] == 18)
                return 1;
            if (old_x - new_x == 2 && old_y - new_y == 2) // 左上
                vector = 1;
            if (new_x - old_x == 2 && old_y - new_y == 2) // 右上
                vector = 2;
            if (old_x - new_x == 2 && new_y - old_y == 2) // 左下
                vector = 3;
            if (new_x - old_x == 2 && new_y - old_y == 2) // 右下
                vector = 4;
            switch (vector)
            {
            case 1:
                if (!chessboard[old_y - 1][old_x - 1])
                    return 1;
                break;
            case 2:
                if (!chessboard[old_y - 1][old_x + 1])
                    return 1;
                break;
            case 3:
                if (!chessboard[old_y + 1][old_x - 1])
                    return 1;
                break;
            case 4:
                if (!chessboard[old_y + 1][old_x + 1])
                    return 1;
                break;
            default:
                break;
            }
        }
        break;
    case 7: // 炮
    case 19:
    case 8:
    case 20:
        int counts = 0;     // 炮走的路上棋子的个数
        if (old_x == new_x) // column相同
        {
            for (int i = min_(old_y, new_y) + 1; i < max_(old_y, new_y); i++)
            {
                if (chessboard[i][old_x])
                    counts++;
            }
        }
        if (old_y == new_y) // row相同
        {
            for (int i = min_(old_x, new_x) + 1; i < max_(old_x, new_x); i++)
            {
                if (chessboard[old_y][i])
                    counts++;
            }
        }
        if (counts > 1 && (chessboard[old_y][old_x] == 7 || chessboard[old_y][old_x] == 19))
            return 0;
        if ((counts != 0 && counts != 2) && (chessboard[old_y][old_x] == 8 || chessboard[old_y][old_x] == 20))
            return 0;
        if (!counts && chessboard[new_y][new_x]) // 炮是不能吃子的
            return 0;
        return 1;
        break;
    case 9: // 兵
    case 21:
    case 10:
    case 22:
        if (!(kind % 2)) // judge自身的情况
        {
            if (old_y >= 5) // 在己方阵地
            {
                if (old_y - new_y == 1)
                    return 1;
                if (old_y - new_y == 2 && (chessboard[old_y][old_x] == 10 || chessboard[old_y][old_x] == 22))
                {
                    if (chessboard[old_y - 1][old_x])
                        return 0;
                    return 1;
                }
            }
            if (old_y < 5) // 在敌方阵地
            {
                if (old_y - new_y == 1 || fabs(new_x - old_x) == 1)
                    return 1;
                if (old_y - new_y == 2 && (chessboard[old_y][old_x] == 10 || chessboard[old_y][old_x] == 22))
                {
                    if (chessboard[old_y - 1][old_x])
                        return 0;
                    return 1;
                }
                if (old_x - new_x == 2 && (chessboard[old_y][old_x] == 10 || chessboard[old_y][old_x] == 22))
                {
                    if (chessboard[old_y][old_x - 1])
                        return 0;
                    return 1;
                }
                if (new_x - old_x == 2 && (chessboard[old_y][old_x] == 10 || chessboard[old_y][old_x] == 22))
                {
                    if (chessboard[old_y][old_x + 1])
                        return 0;
                    return 1;
                }
            }
        }
        if (kind % 2) // judge对方的情况
        {
            if (old_y < 5) // 在己方阵地
            {
                if (new_y - old_y == 1)
                    return 1;
                if (new_y - old_y == 2 && (chessboard[old_y][old_x] == 10 || chessboard[old_y][old_x] == 22))
                {
                    if (chessboard[old_y + 1][old_x])
                        return 0;
                    return 1;
                }
            }
            if (old_y >= 5) // 在敌方阵地
            {
                if (new_y - old_y == 1 || fabs(new_x - old_x) == 1)
                    return 1;
                if (new_y - old_y == 2 && (chessboard[old_y][old_x] == 10 || chessboard[old_y][old_x] == 22))
                {
                    if (chessboard[old_y + 1][old_x])
                        return 0;
                    return 1;
                }
                if (new_x - old_x == 2 && (chessboard[old_y][old_x] == 10 || chessboard[old_y][old_x] == 22))
                {
                    if (chessboard[old_y][old_x + 1])
                        return 0;
                    return 1;
                }
                if (old_x - new_x == 2 && (chessboard[old_y][old_x] == 10 || chessboard[old_y][old_x] == 22))
                {
                    if (chessboard[old_y][old_x - 1])
                        return 0;
                    return 1;
                }
            }
        }
        break;
    case 11: // 士
    case 23:
        if (!(kind % 2)) // judge自身的情况
        {
            if (fabs(old_x - new_x) == 1 && fabs(old_y - new_y) == 1 && new_y >= 7 && new_x >= 3 && new_x <= 5)
                return 1;
        }
        if (kind % 2) // judge对方的情况
        {
            if (fabs(old_x - new_x) == 1 && fabs(old_y - new_y) == 1 && new_y <= 2 && new_x >= 3 && new_x <= 5)
                return 1;
        }
        break;
    case 12: // 将
    case 24:
        if (!(kind % 2)) // judge自身的情况
        {
            if ((fabs(old_x - new_x) == 1 || fabs(old_y - new_y) == 1) && new_y >= 7 && new_x >= 3 && new_x <= 5)
                return 1;
            if (old_x == new_x && chessboard[new_y][new_x] % 12 == 0)
            {
                for (int i = new_y + 1; i < old_y; i++)
                    if (chessboard[i][old_x])
                        return 0;
                return 1;
            }
        }
        if (kind % 2) // judge对方的情况
        {
            if ((fabs(old_x - new_x) == 1 || fabs(old_y - new_y) == 1) && new_y <= 2 && new_x >= 3 && new_x <= 5)
                return 1;
            if (old_x == new_x && chessboard[new_y][new_x] % 12 == 0)
            {
                for (int i = old_y + 1; i < new_y; i++)
                    if (chessboard[i][old_x])
                        return 0;
                return 1;
            }
        }
        break;
    default:
        break;
    }
    return 0;
}
int JudgeWinOrLose(int chessboard[][9])
{
    int red = 0, black = 0;
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 9; j++)
        {
            if (chessboard[i][j] == 12)
                red = 1;
            if (chessboard[i][j] == 24)
                black = 1;
        }
    if (!red)
        return 1; // 红方输了
    if (!black)
        return 2; // 黑方输了
    return 0;     // 还在对战中
}
// itoa(int value,char *str,int base) 数字转字符

// 正为红，负为黑，奇数为正常棋子，偶数为魔法棋子

int times = 0;

#endif