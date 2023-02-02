#ifndef WHETHER_BE_CITED_OR_NOT
#define WHETHER_BE_CITED_OR_NOT

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"

#undef main

void Load();
void Quit();
int Play();
void PrintAll();
void PrintPicture_first_level();
void SendToTheBox(int kind_of_picture);
void PrintPicture_second_level();
void Restart();
void Pause();
void RandomCreate();
void RandomCreate2();
void JudgeIfOver();
void Click(int zone);
void OperateAndJudge();
void PrintThePictureInTheBox();
void DisLocate_first_level();
void MakeRand(int arr[], int count);
void MakeExtraSpace();
int JudgeSpace();
void PrintExtraSpace();
int JudgeIfBeCovered(int relative_position);
void DisLocate_second_level();
void OperateTheExtraSpace(int i);
void PrintResurrectSpace();

static int dialog = 0; // 没有对话框为0，有停止对话框为1，有胜利对话框1为2，有胜利对话框2为3
static int first_level_new[9][3];
static int first_level_old[9][3];
static int store_new[10];
static int store_old[10];
static int location_first_level[27][2];
static int array_for_dislocate[180] = {0};
static int array_for_dislocate2[180] = {0};
static int array_for_more_space_new[3] = {-1, -1, -1};
static int array_for_more_space_old[3] = {-1, -1, -1};
static int array_for_resurrect_new[3] = {-1, -1, -1};
static int array_for_resurrect_old[3] = {-1, -1, -1};
static int second_level_new[99][3];
static int second_level_old[99][3];
static int if_input = 0, if_input_2 = 0;
int level = 0; // level == 0 -- mainbackground level == 1 -- the first level level == 2 transition level == 3 the second level level == 4 die for the first time level == 5 die for the second time level == 6 win
int ifwithdraw = 0;
int judgeclick = 0;
int ifhavemadeextraspace = 0;
int wheretoexit = 0;
int dietimes = 0;
int whetherresurrect = 0;
int whetherexit = 0;

static SDL_Window *Window = NULL;                  // 初始化好一个窗口
static SDL_Renderer *Renderer = NULL;              // 初始化好一个“画笔"
static SDL_Surface *MainBackGroundSurface = NULL;  // 进入页面背景
static SDL_Surface *PlayBackGroundSurface = NULL;  // 第一关背景
static SDL_Surface *PlayBackGround2Surface = NULL; // 第二关背景
static SDL_Surface *PauseSurface = NULL;           // 对话框(pause)
static SDL_Surface *NoteSurface[3] = {NULL};       // 第一关三种图标
static SDL_Surface *NoteSurface2[11] = {NULL};     // 第二关十种图标
static SDL_Surface *TransitionSurface = NULL;      // 过渡界面
static SDL_Surface *Fail1Surface = NULL;           // 失败一次
static SDL_Surface *Fail2Surface = NULL;           // 失败两次
static SDL_Surface *WinSurface = NULL;             // 胜利
static SDL_Rect NoteRect;                          // 图标的坐标与大小信息
static Mix_Music *Bgm = NULL;                      // 音乐
static SDL_Event MainEvent;                        // 开始事件
static SDL_Event PlayEvent;                        // 游戏事件

#endif // WHETHER_BE_CITED_OR_NOT