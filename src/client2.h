#ifndef WHETHER_BE_CITED_OR_NOT_CLIENT2
#define WHETHER_BE_CITED_OR_NOT_CLIENT2

#include <time.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"
#include <sys/time.h>

void QUIT();
void Load();
void Paint_Ready();
int Play();
void Paint_Chessboard();
void PrintAll();
void PaintTheChessPieces();
void Paint_Already_Ready();
void PaintTheFrame();
void Store(int chessboard_new[][9], int chessboard_old[][9]);
void PaintHumo();
void PaintWinOrLose();
void PaintRegret();
void Again();

static SDL_Window *Window = NULL;                 // 初始化好一个窗口
static SDL_Renderer *Renderer = NULL;             // 初始化好一个“画笔"
static Mix_Music *Bgm = NULL;                     // 音乐
static SDL_Event MainEvent;                       // 开始事件
static SDL_Event PlayEvent;                       // 游戏事件
static SDL_Event e;                               // 鼠标事件
static SDL_Surface *MainBackGroundSurface = NULL; // 进入页面背景
static SDL_Rect NoteRect;                         // 图标的坐标与大小信息
static SDL_Surface *NoteSurface[24] = {NULL};     // 24种棋子形态
static SDL_Surface *ReadySurface = NULL;          // 准备
static SDL_Surface *Already_ReadySurface = NULL;  // 已准备
static SDL_Surface *FrameSurface = NULL;          // 选中框
static SDL_Surface *HumoSurface = NULL;           // 护摩之杖
static SDL_Surface *WinSurface[2] = {NULL};       // 输赢形态
static SDL_Surface *AgainSurface = NULL;          // 再来一局
static SDL_Surface *RegretSurface = NULL;         // 再来一局

static int ready = 0;
static int socket_server;
static int chessboard_black_copy[10][9] = {1, 3, 5, 11, 12, 11, 5, 3, 1,
                                           0, 0, 0, 0, 0, 0, 0, 0, 0,
                                           0, 7, 0, 0, 0, 0, 0, 7, 0,
                                           9, 0, 9, 0, 9, 0, 9, 0, 9,
                                           0, 0, 0, 0, 0, 0, 0, 0, 0,
                                           0, 0, 0, 0, 0, 0, 0, 0, 0,
                                           21, 0, 21, 0, 21, 0, 21, 0, 21,
                                           0, 19, 0, 0, 0, 0, 0, 19, 0,
                                           0, 0, 0, 0, 0, 0, 0, 0, 0,
                                           13, 15, 17, 23, 24, 23, 17, 15, 13};
static int chessboard_black_new[10][9] = {1, 3, 5, 11, 12, 11, 5, 3, 1,
                                          0, 0, 0, 0, 0, 0, 0, 0, 0,
                                          0, 7, 0, 0, 0, 0, 0, 7, 0,
                                          9, 0, 9, 0, 9, 0, 9, 0, 9,
                                          0, 0, 0, 0, 0, 0, 0, 0, 0,
                                          0, 0, 0, 0, 0, 0, 0, 0, 0,
                                          21, 0, 21, 0, 21, 0, 21, 0, 21,
                                          0, 19, 0, 0, 0, 0, 0, 19, 0,
                                          0, 0, 0, 0, 0, 0, 0, 0, 0,
                                          13, 15, 17, 23, 24, 23, 17, 15, 13};
static int chessboard_black_old[10][9]; // 黑方视角
static int start = 0;
static char x_old_black[4], y_old_black[4], x_new_black[4], y_new_black[4];
static char x_old_red[4], y_old_red[4], x_new_red[4], y_new_red[4];
static int wait = 0;
static int which_to_receive = 0;
static int turn = 0; // 0代表不是自己的回合，1代表是自己的回合
static int whether_get_power = 0;
static int lose = 0;             // 0代表还在对战中，1代表输了，2代表赢了
static int whether_replying = 0; // 是否收到了悔棋申请
static int black = 0;
static int self_ready = 0;
static int opp_ready = 0;

#endif