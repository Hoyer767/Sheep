#include "sheep.h"

int main(int argc, char *argv[]) /*控制主界面*/
{
    printf("MainEvent\n");
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO); // 初始化
    Mix_Init(MIX_INIT_MP3);
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
    Window = SDL_CreateWindow("sheep", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 900, SDL_WINDOW_SHOWN); /*创建窗口*/
    Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED); /*创建画笔*/                               /*定义主事件*/

    Load(); // 加载图片和文件
    Mix_PlayMusic(Bgm, -1);

    SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
    while (SDL_WaitEvent(&MainEvent))
    {
        switch (MainEvent.type)
        {
        case SDL_QUIT: // 退出(叉)
            Quit();
            return 0;
        case SDL_KEYDOWN:
            switch (MainEvent.key.keysym.sym) /*键盘交互界面*/
            {
            case SDLK_RETURN:
            case SDLK_SPACE:
                if (wheretoexit == 1)
                    level = 1;
                if (wheretoexit == 2)
                    level = 3;
                if (wheretoexit == 0)
                {
                    level = 1;
                    RandomCreate();
                    RandomCreate2();
                }
                if (Play())
                    return 0;
                break;
            case SDLK_ESCAPE:
                Quit();
                return 0;
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN: /*按下鼠标*/
            printf("(%d,%d)\n", MainEvent.button.x, MainEvent.button.y);
            break;
        case SDL_MOUSEBUTTONUP: /*抬起鼠标*/
            printf("(%d,%d)\n", MainEvent.button.x, MainEvent.button.y);
            if (MainEvent.button.x >= 158 && MainEvent.button.x <= 444 && MainEvent.button.y >= 232 && MainEvent.button.y <= 333)
            {
                if (wheretoexit == 2)
                    level = 3;
                if (wheretoexit == 1)
                    level = 1;
                if (wheretoexit == 0)
                {
                    level = 1;
                    RandomCreate();
                    RandomCreate2();
                }
                Play();
            }
            if (MainEvent.button.x >= 160 && MainEvent.button.x <= 443 && MainEvent.button.y >= 393 && MainEvent.button.y <= 490 && level == 0)
            {
                if (whetherexit == 0)
                    Quit();
                whetherexit = 0;
            }
            break;
        case SDL_MOUSEMOTION: /*移动鼠标*/
            break;
        default:
            break;
        }
        SDL_Texture *MainBackGroundTexture = SDL_CreateTextureFromSurface(Renderer, MainBackGroundSurface);
        SDL_RenderCopy(Renderer, MainBackGroundTexture, NULL, NULL);
        SDL_RenderPresent(Renderer);
        SDL_DestroyTexture(MainBackGroundTexture);
    }
    Quit();
    return 0;
}

void Load()
{
    MainBackGroundSurface = IMG_Load("pictures/mainbackground.png");
    PlayBackGroundSurface = IMG_Load("pictures/playbackground.png");
    PlayBackGround2Surface = IMG_Load("pictures/playbackground2.png");
    PauseSurface = IMG_Load("pictures/pause_background.png");
    TransitionSurface = IMG_Load("pictures/transition.png");
    Fail1Surface = IMG_Load("pictures/die1.png");
    Fail2Surface = IMG_Load("pictures/die2.png");
    WinSurface = IMG_Load("pictures/win.png");
    Bgm = Mix_LoadMUS("bgm.mp3");
    for (int i = 0; i < 3; i++)
    {
        char BoxName[20];
        sprintf(BoxName, "pictures/picture%d.png", i);
        NoteSurface[i] = IMG_Load(BoxName);
    }
    for (int i = 0; i < 11; i++)
    {
        char BoxName[20];
        sprintf(BoxName, "pictures/picture_2_%d.png", i);
        NoteSurface2[i] = IMG_Load(BoxName);
    }
}

void Quit()
{
    SDL_FreeSurface(MainBackGroundSurface);
    SDL_FreeSurface(PlayBackGroundSurface);
    SDL_FreeSurface(PauseSurface);
    SDL_FreeSurface(TransitionSurface);
    SDL_FreeSurface(PlayBackGround2Surface);
    SDL_FreeSurface(Fail1Surface);
    SDL_FreeSurface(Fail2Surface);
    SDL_FreeSurface(WinSurface);
    for (int i = 0; i < 3; i++)
        SDL_FreeSurface(NoteSurface[i]);
    for (int i = 0; i < 11; i++)
        SDL_FreeSurface(NoteSurface2[i]);
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);
    Mix_FreeMusic(Bgm);
    Mix_Quit();
    SDL_Quit();
}

int Play()
{
    printf("PlayEvent\n");
    while (1)
    {
        PrintAll(); // 这一段也在一直被判断；
        while (SDL_PollEvent(&PlayEvent) || dialog)
        {
            switch (PlayEvent.type)
            {
            case SDL_QUIT:
                Quit();
                return 1;
            case SDL_KEYDOWN:
                switch (PlayEvent.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    Quit();
                    return 1;
                case SDLK_r:
                    Restart(); /*重开*/
                    break;
                case SDLK_RETURN:
                    if (level == 2)
                        level = 3;
                    break;
                default:
                    break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                printf("(%d,%d)\n", PlayEvent.button.x, PlayEvent.button.y);
                break;
            case SDL_MOUSEBUTTONUP:
                printf("(%d,%d)\n", PlayEvent.button.x, PlayEvent.button.y);
                if (!dialog)
                {
                    if (PlayEvent.button.x >= 164 && PlayEvent.button.x <= 448 && PlayEvent.button.y >= 430 && PlayEvent.button.y <= 473 && level == 4)
                    {
                        ifhavemadeextraspace = 1; // 用来判断能不能进行撤回操作
                        MakeExtraSpace();
                        level = 3;
                        whetherresurrect = 1;
                    } // resurrect
                    for (int i = 0; i < 99; i++)
                    {
                        if (PlayEvent.button.x >= 195 && PlayEvent.button.x <= 417 && PlayEvent.button.y >= 426 && PlayEvent.button.y <= 469 && level == 3 && whetherresurrect == 1)
                        {
                            whetherresurrect = 0;
                            break;
                        }
                        if (PlayEvent.button.x >= second_level_new[i][0] && PlayEvent.button.x <= second_level_new[i][0] + 45 && PlayEvent.button.y >= second_level_new[i][1] && PlayEvent.button.y <= second_level_new[i][1] + 45 && level == 3 && second_level_new[i][2] != -1)
                        {
                            ifwithdraw = 0;
                            judgeclick = 1; // 上部
                            ifhavemadeextraspace = 0;
                            if (!JudgeIfBeCovered(i)) // 没被覆盖
                            {
                                for (int i = 0; i < 99; i++)
                                    second_level_old[i][2] = second_level_new[i][2]; // 保存数组状态
                                int kind_of_picture = second_level_new[i][2];
                                second_level_new[i][2] = -1;
                                SendToTheBox(kind_of_picture);
                                break;
                            }
                        } // 点到了这个方块而且它还存在
                    }     // 第二关的点击事件}
                    if (PlayEvent.button.x >= 534 && PlayEvent.button.x <= 590 && PlayEvent.button.y >= 18 && PlayEvent.button.y <= 70 && level != 2)
                    {
                        dialog = 1;
                        Pause();
                    }
                    for (int i = 0; i < 27; i++)
                    {
                        if (PlayEvent.button.x > location_first_level[i][0] && PlayEvent.button.x < location_first_level[i][0] + 50 && PlayEvent.button.y > location_first_level[i][1] && PlayEvent.button.y < location_first_level[i][1] + 50 && level == 1)
                        {
                            ifwithdraw = 0;
                            judgeclick = 1; // 上部
                            ifhavemadeextraspace = 0;
                            Click(((PlayEvent.button.y - 100) / 150) * 3 + (PlayEvent.button.x - 160) / 120);
                            break;
                        }
                    } // 第一关的点击事件
                    if (level == 2 && PlayEvent.button.x >= 246 && PlayEvent.button.x <= 364 && PlayEvent.button.y >= 591 && PlayEvent.button.y <= 638)
                    {
                        level = 3;
                    }
                    if (PlayEvent.button.x >= 53 && PlayEvent.button.x <= 169 && PlayEvent.button.y >= 766 && PlayEvent.button.y <= 853) // 挪出三个空位
                    {
                        ifhavemadeextraspace = 1; // 用来判断能不能进行撤回操作
                        if (JudgeSpace())
                            MakeExtraSpace();
                    }
                    if (PlayEvent.button.x >= 244 && PlayEvent.button.x <= 359 && PlayEvent.button.y >= 766 && PlayEvent.button.y <= 853 && ifhavemadeextraspace == 0) // 撤回一步
                    {
                        if (level == 1 && ifwithdraw == 0 && judgeclick == 1)
                        {
                            for (int k = 0; k < 9; k++)
                            {
                                for (int j = 0; j < 3; j++)
                                {
                                    first_level_new[k][j] = first_level_old[k][j];
                                }
                            }
                        }
                        if (level == 3 && ifwithdraw == 0 && judgeclick == 1)
                        {
                            for (int i = 0; i < 99; i++)
                                second_level_new[i][2] = second_level_old[i][2];
                        }
                        if (judgeclick == 2)
                        {
                            for (int i = 0; i < 3; i++)
                                array_for_more_space_new[i] = array_for_more_space_old[i];
                        } // extraspace
                        if (judgeclick == 3)
                        {
                            for (int i = 0; i < 3; i++)
                                array_for_resurrect_new[i] = array_for_resurrect_old[i];
                        } // resurrect
                        for (int i = 0; i < 10; i++)
                            store_new[i] = store_old[i];
                        ifwithdraw = 1;
                    }
                    if (PlayEvent.button.x >= 432 && PlayEvent.button.x <= 548 && PlayEvent.button.y >= 766 && PlayEvent.button.y <= 853) // 重组
                    {
                        if (level == 1)
                            DisLocate_first_level();
                        if (level == 3)
                            DisLocate_second_level();
                    }
                    for (int i = 0; i < 3; i++) // 点击多余的三个空间时的处理
                    {
                        if (PlayEvent.button.x >= 216 + 60 * i && PlayEvent.button.x <= 267 + 60 * i && PlayEvent.button.y >= 543 && PlayEvent.button.y <= 597 && level == 1)
                            OperateTheExtraSpace(i);
                        if (PlayEvent.button.x >= 145 + 45 * i && PlayEvent.button.x <= 190 + 45 * i && PlayEvent.button.y >= 543 && PlayEvent.button.y <= 597 && level == 3)
                            OperateTheExtraSpace(i);
                    } // 点击extraspace
                    for (int i = 0; i < 3; i++)
                    {
                        if (PlayEvent.button.x >= 326 + 45 * i && PlayEvent.button.x <= 371 + 45 * i && PlayEvent.button.y >= 543 && PlayEvent.button.y <= 597 && level == 3)
                        {
                            judgeclick = 3; // 复活区
                            ifhavemadeextraspace = 0;
                            ifwithdraw = 0;
                            SendToTheBox(array_for_resurrect_new[i]);
                            for (int j = 0; j < 3; j++)
                                array_for_resurrect_old[j] = array_for_resurrect_new[j]; // 保存一下子
                            array_for_resurrect_new[i] = -1;
                        }
                    }
                    if (PlayEvent.button.x >= 243 && PlayEvent.button.x <= 371 && PlayEvent.button.y >= 546 && PlayEvent.button.y <= 594 && level == 4)
                    {
                        wheretoexit = 0;
                        level = 0;
                        dietimes = 0;
                        return 0;
                    } // exit
                    if (PlayEvent.button.x >= 232 && PlayEvent.button.x <= 358 && PlayEvent.button.y >= 607 && PlayEvent.button.y <= 650 && level == 6)
                    {
                        wheretoexit = 0;
                        level = 0;
                        dietimes = 0;
                        return 0;
                    } // exit
                    if (PlayEvent.button.x >= 235 && PlayEvent.button.x <= 364 && PlayEvent.button.y >= 544 && PlayEvent.button.y <= 585 && level == 5)
                    {
                        wheretoexit = 0;
                        level = 0;
                        dietimes = 0;
                        return 0;
                    } // exit
                }
                else
                {
                    if (PlayEvent.button.x >= 244 && PlayEvent.button.x <= 404 && PlayEvent.button.y >= 479 && PlayEvent.button.y <= 499)
                    {
                        Restart();
                    }
                    if (PlayEvent.button.x >= 275 && PlayEvent.button.x <= 380 && PlayEvent.button.y >= 414 && PlayEvent.button.y <= 438)
                    {
                        dialog = 0;
                        if (level == 1)
                            wheretoexit = 1; // 从第一关出来
                        if (level == 3)
                            wheretoexit = 2; // 从第二关出来
                        level = 0;
                        whetherexit = 1;
                        return 0;
                    }
                    if (PlayEvent.button.x >= 206 && PlayEvent.button.x <= 443 && PlayEvent.button.y >= 348 && PlayEvent.button.y <= 371)
                    {
                        dialog = 0;
                    }
                }
                break;
            default:
                break;
            }
        }
    }
}

void OperateTheExtraSpace(int i)
{
    judgeclick = 2; // 下部
    ifhavemadeextraspace = 0;
    ifwithdraw = 0;
    SendToTheBox(array_for_more_space_new[i]);
    for (int j = 0; j < 3; j++)
        array_for_more_space_old[j] = array_for_more_space_new[j];
    array_for_more_space_new[i] = -1;
}

int JudgeIfBeCovered(int relative_position)
{
    int keypoint_x, keypoint_y;
    for (int i = relative_position + 1; i < 99; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            switch (j)
            {
            case 0:
                keypoint_x = second_level_new[i][0] + 1;
                keypoint_y = second_level_new[i][1] + 1;
                break;
            case 1:
                keypoint_x = second_level_new[i][0] + 43;
                keypoint_y = second_level_new[i][1] + 1;
                break;
            case 2:
                keypoint_x = second_level_new[i][0] + 1;
                keypoint_y = second_level_new[i][1] + 43;
                break;
            case 3:
                keypoint_x = second_level_new[i][0] + 43;
                keypoint_y = second_level_new[i][1] + 43;
                break;
            default:
                break;
            }
            if (keypoint_x >= second_level_new[relative_position][0] && keypoint_x <= second_level_new[relative_position][0] + 45 && keypoint_y >= second_level_new[relative_position][1] && keypoint_y <= second_level_new[relative_position][1] + 45 && second_level_new[i][2] != -1)
                return 1; // 被覆盖了 // 后打印出来的图片的某一个角落在了选中的图片上
        }
    }
    return 0; // 没有被覆盖
}

void PrintAll()
{
    if (!dialog && level == 1)
    {
        PrintPicture_first_level();
        PrintExtraSpace();
        PrintThePictureInTheBox();
    }
    if (!dialog && level == 3)
    {
        PrintPicture_second_level();
        PrintExtraSpace();
        PrintResurrectSpace();
        PrintThePictureInTheBox();
    }
    if (level == 1)
    {
        SDL_Texture *PlayBackGroundTexture = SDL_CreateTextureFromSurface(Renderer, PlayBackGroundSurface);
        SDL_RenderCopy(Renderer, PlayBackGroundTexture, NULL, NULL);
        SDL_DestroyTexture(PlayBackGroundTexture);
    }
    if (level == 2)
    {
        SDL_Texture *TransitionTexture = SDL_CreateTextureFromSurface(Renderer, TransitionSurface);
        SDL_RenderCopy(Renderer, TransitionTexture, NULL, NULL);
        SDL_DestroyTexture(TransitionTexture);
        SDL_RenderPresent(Renderer);
        Sleep(1500);
        level = 3;
    }
    if (level == 3)
    {
        SDL_Texture *PlayBackGround2Texture = SDL_CreateTextureFromSurface(Renderer, PlayBackGround2Surface);
        SDL_RenderCopy(Renderer, PlayBackGround2Texture, NULL, NULL);
        SDL_DestroyTexture(PlayBackGround2Texture);
    }
}

void PrintPicture_first_level()
{
    int location_row = 0;
    for (int i = 0; i < 9; i++)
    {
        NoteRect.x = 160 + (i % 3) * 120;
        NoteRect.y = 100 + (i / 3) * 150;
        for (int j = 0; j < 3; j++)
        {
            int location_column = 0;
            if (first_level_new[i][j] == -1)
                continue;
            NoteRect.y += 10;
            NoteRect.w = NoteSurface[first_level_new[i][j]]->w + 5; // 50像素
            NoteRect.h = NoteSurface[first_level_new[i][j]]->h + 5; // 50像素
            SDL_Texture *NoteTexture = SDL_CreateTextureFromSurface(Renderer, NoteSurface[first_level_new[i][j]]);
            SDL_RenderCopy(Renderer, NoteTexture, NULL, &NoteRect);
            SDL_DestroyTexture(NoteTexture);
            if (!if_input)
            {
                location_first_level[location_row][location_column] = NoteRect.x;
                location_first_level[location_row++][++location_column] = NoteRect.y;
            }
        }
    }
    if_input = 1;
}

void PrintPicture_second_level()
{
    NoteRect.x = 100;
    NoteRect.y = 80;
    for (int i = 0; i < 44; i++)
    {
        NoteRect.y += 8;
        switch (i)
        {
        case 6:
        case 18:
            NoteRect.y -= 48;
            NoteRect.x += 360;
            break;
        case 12:
        case 24:
            NoteRect.y += 80;
            NoteRect.x -= 360;
            break;
        case 34:
            NoteRect.y -= 80;
            NoteRect.x += 360;
            break;
        default:
            break;
        }
        if (second_level_new[i][2] == -1)
            continue;
        NoteRect.w = NoteSurface2[second_level_new[i][2]]->w + 5; // 50像素
        NoteRect.h = NoteSurface2[second_level_new[i][2]]->h + 5; // 50像素
        SDL_Texture *NoteTexture = SDL_CreateTextureFromSurface(Renderer, NoteSurface2[second_level_new[i][2]]);
        SDL_RenderCopy(Renderer, NoteTexture, NULL, &NoteRect);
        SDL_DestroyTexture(NoteTexture);
        if (!if_input_2)
        {
            second_level_new[i][0] = NoteRect.x;
            second_level_new[i][1] = NoteRect.y;
        }
    } // 绘制左右的44个图标(44)
    NoteRect.x = 235;
    NoteRect.y = 150;
    for (int i = 0; i < 3; i++)
    {
        NoteRect.x -= (i == 0) ? 0 : 90;
        NoteRect.y += (i == 0) ? 0 : 150;
        for (int j = 0; j < 3; j++)
        {
            NoteRect.x += (j == 0) ? 0 : 45;
            if (second_level_new[44 + i * 3 + j][2] == -1)
                continue;
            NoteRect.w = NoteSurface2[second_level_new[44 + i * 3 + j][2]]->w + 5; // 50像素
            NoteRect.h = NoteSurface2[second_level_new[44 + i * 3 + j][2]]->h + 5; // 50像素
            SDL_Texture *NoteTexture = SDL_CreateTextureFromSurface(Renderer, NoteSurface2[second_level_new[44 + i * 3 + j][2]]);
            SDL_RenderCopy(Renderer, NoteTexture, NULL, &NoteRect);
            SDL_DestroyTexture(NoteTexture);
            if (!if_input_2)
            {
                second_level_new[44 + i * 3 + j][0] = NoteRect.x;
                second_level_new[44 + i * 3 + j][1] = NoteRect.y;
            }
        }
    } // 绘制最底层的3 * 3个图标(53)
    NoteRect.x = 350;
    NoteRect.y = 25;
    for (int i = 0; i < 4; i++)
    {
        NoteRect.x -= 135;
        NoteRect.y += (i % 2 == 0) ? 100 : 50;
        for (int j = 0; j < 4; j++)
        {
            NoteRect.x += (j == 0) ? 0 : 45;
            if (second_level_new[53 + 4 * i + j][2] == -1)
                continue;
            NoteRect.w = NoteSurface2[second_level_new[53 + 4 * i + j][2]]->w + 5; // 50像素
            NoteRect.h = NoteSurface2[second_level_new[53 + 4 * i + j][2]]->h + 5; // 50像素
            SDL_Texture *NoteTexture = SDL_CreateTextureFromSurface(Renderer, NoteSurface2[second_level_new[53 + 4 * i + j][2]]);
            SDL_RenderCopy(Renderer, NoteTexture, NULL, &NoteRect);
            SDL_DestroyTexture(NoteTexture);
            if (!if_input_2)
            {
                second_level_new[53 + 4 * i + j][0] = NoteRect.x;
                second_level_new[53 + 4 * i + j][1] = NoteRect.y;
            }
        }
    } // 绘制中层的16个图标(69)
    NoteRect.x = 371;
    NoteRect.y = 40;
    for (int i = 0; i < 6; i++)
    {
        NoteRect.x -= 180;
        switch (i)
        {
        case 0:
            NoteRect.y += 72;
            break;
        case 1:
            NoteRect.y += 72;
            break;
        case 2:
            NoteRect.y += 72;
            break;
        case 3:
            NoteRect.y += 85;
            break;
        case 4:
            NoteRect.y += 85;
            break;
        case 5:
            NoteRect.y += 50;
            break;
        default:
            break;
        }
        for (int j = 0; j < 5; j++)
        {
            NoteRect.x += (j == 0) ? 0 : 45;
            if (second_level_new[69 + 5 * i + j][2] == -1)
                continue;
            NoteRect.w = NoteSurface2[second_level_new[69 + 5 * i + j][2]]->w + 5; // 50像素
            NoteRect.h = NoteSurface2[second_level_new[69 + 5 * i + j][2]]->h + 5; // 50像素
            SDL_Texture *NoteTexture = SDL_CreateTextureFromSurface(Renderer, NoteSurface2[second_level_new[69 + 5 * i + j][2]]);
            SDL_RenderCopy(Renderer, NoteTexture, NULL, &NoteRect);
            SDL_DestroyTexture(NoteTexture);
            if (!if_input_2)
            {
                second_level_new[69 + 5 * i + j][0] = NoteRect.x;
                second_level_new[69 + 5 * i + j][1] = NoteRect.y;
            }
        }
    } // 绘制上层的30个图标(99)
    if_input_2 = 1;
}

void Restart()
{
    if (level == 1)
        RandomCreate();
    if (level == 3)
        RandomCreate2();
    dialog = 0;
}

void Pause()
{
    SDL_Texture *PauseTexture = SDL_CreateTextureFromSurface(Renderer, PauseSurface);
    SDL_RenderCopy(Renderer, PauseTexture, NULL, NULL);
    SDL_DestroyTexture(PauseTexture);
    SDL_RenderPresent(Renderer);
}

void RandomCreate() // 将第一关的卡片随机初始化
{
    int k = 0, r, j, q;
    srand((int)time(0));
    if (level == 1)
    {
        while (k < 27)
        {
            int judge = 1, index = 0, judge1 = 1;
            r = rand() % 27 + 1;
            for (int i = 0; i < 9; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    if (index == k)
                    {
                        judge1 = 0;
                        break;
                    }
                    if (r == first_level_new[i][j])
                    {
                        judge = 0;
                        break;
                    }
                    index++;
                }
                if (!judge || !judge1)
                    break;
            }
            if (judge)
            {
                first_level_new[k / 3][k % 3] = r;
                k++;
            }
        }

        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                first_level_new[i][j] %= 3;
                first_level_old[i][j] = first_level_new[i][j];
            }
        }
    } // 初始化第一关
    for (int i = 0; i < 10; i++)
    {
        store_new[i] = -1;
        store_old[i] = -1;
    } // 初始化box
    for (int i = 0; i < 3; i++)
        array_for_more_space_new[i] = -1; // 初始化多余空间
    ifwithdraw = 0;                       // 初始化撤回状态
    judgeclick = 0;                       // 初始化点击区域
    ifhavemadeextraspace = 0;             // 此时可以创造多余空间
}

void RandomCreate2()
{
    int j = 0, q;
    while (j < 99)
    {
        int judge = 1, index = 0, judge1 = 1;
        q = rand() % 99 + 1;
        for (int i = 0; i < 99; i++)
        {
            if (index == j)
            {
                judge1 = 0;
                break;
            }
            if (q == second_level_new[i][2])
            {
                judge = 0;
                break;
            }
            index++;
            if (!judge || !judge1)
                break;
        }
        if (judge)
        {
            second_level_new[j][2] = q;
            j++;
        }
    }
    for (int i = 0; i < 99; i++)
    {
        second_level_new[i][2] %= 11;
        second_level_old[i][2] = second_level_new[i][2];
    } // 初始化第二关
    for (int i = 0; i < 10; i++)
    {
        store_new[i] = -1;
        store_old[i] = -1;
    } // 初始化box
    for (int i = 0; i < 3; i++)
    {
        array_for_more_space_new[i] = -1;
        array_for_resurrect_new[i] = -1;
    }                         // 初始化多余空间和复活空间
    ifwithdraw = 0;           // 初始化撤回状态
    judgeclick = 0;           // 初始化点击区域
    ifhavemadeextraspace = 0; // 此时可以创造多余空间
    dietimes = 0;             // 初始化失败次数
}

void Click(int zone)
{
    int kind_of_picture = -1;
    for (int i = 2; i >= 0; i--)
    {
        if (first_level_new[zone][i] != -1)
        {
            kind_of_picture = first_level_new[zone][i];
            for (int k = 0; k < 9; k++)
                for (int j = 0; j < 3; j++)
                    first_level_old[k][j] = first_level_new[k][j];
            first_level_new[zone][i] = -1;
            break;
        }
    }
    SendToTheBox(kind_of_picture);
}

void SendToTheBox(int kind_of_picture)
{
    for (int i = 0; i < 10; i++)
        store_old[i] = store_new[i]; // 存储store_new
    for (int i = 0; i < 3; i++)
        array_for_more_space_old[i] = array_for_more_space_new[i]; // 存储多出的三个空间的状态
    if (kind_of_picture == -1)
        return;
    // 先判断有没有相同的图标
    int i, judge = 0; /*最终judge为0说明没有相同图标，如果judge为1说明有相同图标*/
    for (i = 0; i < 10; i++)
    {
        if (kind_of_picture == store_new[i])
        {
            judge = 1;
            break;
        }
        if (store_new[i] == -1)
            break;
    }
    if (judge)
    {
        for (int j = 6; j > i; j--)
            store_new[j] = store_new[j - 1];
    }
    store_new[i] = kind_of_picture;
    OperateAndJudge();
}

void OperateAndJudge()
{
    int sign = 0, i; /*sign为零说明失败，sign为2说明有三个连续的*/
    for (i = 0; i < 7; i++)
    {
        if (store_new[i + 1] == store_new[i])
            sign++;
        else
            sign = 0;
        if (sign == 2)
            break;
    }
    if (sign != 2 && level == 3)
        JudgeIfOver();
    if (sign == 2)
    {
        for (int j = i; j <= 6; j++)
            store_new[j - 1] = store_new[j + 2];
        for (int k = 6; k < 10; k++)
            store_new[k] = -1;
    }
    if (level == 1 || (level == 3 && sign == 2))
        JudgeIfOver();
}

void PrintThePictureInTheBox()
{
    for (int i = 0; i < 10; i++)
    {
        if (store_new[i] == -1)
            break;
        NoteRect.x = 30 + 80 * i;
        NoteRect.y = 645;
        if (level == 1)
        {
            NoteRect.w = NoteSurface[store_new[i]]->w + 5;
            NoteRect.h = NoteSurface[store_new[i]]->h + 5;
            SDL_Texture *NoteTexture = SDL_CreateTextureFromSurface(Renderer, NoteSurface[store_new[i]]);
            SDL_RenderCopy(Renderer, NoteTexture, NULL, &NoteRect);
            SDL_DestroyTexture(NoteTexture);
        }
        if (level == 3)
        {
            NoteRect.w = NoteSurface2[store_new[i]]->w + 5;
            NoteRect.h = NoteSurface2[store_new[i]]->h + 5;
            SDL_Texture *NoteTexture = SDL_CreateTextureFromSurface(Renderer, NoteSurface2[store_new[i]]);
            SDL_RenderCopy(Renderer, NoteTexture, NULL, &NoteRect);
            SDL_DestroyTexture(NoteTexture);
        }
    }
    SDL_RenderPresent(Renderer); /*这个present在这边，不知道为什么它出现多次图片就会闪*/
}

void JudgeIfOver()
{
    int judge = 1, judgebox = 1;
    if (level == 1)
    {
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (first_level_new[i][j] != -1)
                {
                    judge = 0;
                    break;
                }
            }
        }
    }
    if (level == 3)
    {
        for (int i = 0; i < 7; i++)
        {
            if (store_new[i] == -1)
            {
                judgebox = 0;
                break;
            }
        }
        for (int i = 0; i < 99; i++)
        {
            if (second_level_new[i][2] != -1)
            {
                judge = 0;
                break;
            }
        }
    }
    for (int i = 0; i < 7; i++)
        if (store_new[i] != -1)
        {
            judge = 0;
            break;
        }
    if (judge && level == 1)
    {
        for (int i = 0; i < 10; i++)
        {
            store_new[i] = -1;
            store_old[i] = -1;
        } // 初始化box
        level = 2;
    }
    if (judge && level == 3)
    {
        level = 6;
        SDL_Texture *WinTexture = SDL_CreateTextureFromSurface(Renderer, WinSurface);
        SDL_RenderCopy(Renderer, WinTexture, NULL, NULL);
        SDL_DestroyTexture(WinTexture);
        SDL_RenderPresent(Renderer);
    } // 胜利画面
    if (judgebox && dietimes && level == 3)
    {
        level = 5;
        SDL_Texture *Fail2Texture = SDL_CreateTextureFromSurface(Renderer, Fail2Surface);
        SDL_RenderCopy(Renderer, Fail2Texture, NULL, NULL);
        SDL_DestroyTexture(Fail2Texture);
        SDL_RenderPresent(Renderer);
    } // 失败过一次
    if (judgebox && !dietimes && level == 3)
    {
        dietimes++;
        SDL_Texture *Fail1Texture = SDL_CreateTextureFromSurface(Renderer, Fail1Surface);
        SDL_RenderCopy(Renderer, Fail1Texture, NULL, NULL);
        SDL_DestroyTexture(Fail1Texture);
        SDL_RenderPresent(Renderer);
        level = 4;
    } // 没失败过
}

void DisLocate_first_level()
{
    int count = 0;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (first_level_new[i][j] != -1)
                array_for_dislocate[count++] = first_level_new[i][j];
        }
    }
    MakeRand(array_for_dislocate, count);
    count = 0;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (first_level_new[i][j] != -1)
                first_level_new[i][j] = array_for_dislocate[count++];
        }
    }
}

void DisLocate_second_level()
{
    int count = 0;
    for (int i = 0; i < 99; i++)
        if (second_level_new[i][2] != -1)
            array_for_dislocate2[count++] = second_level_new[i][2];
    MakeRand(array_for_dislocate2, count);
    count = 0;
    for (int i = 0; i < 99; i++)
        if (second_level_new[i][2] != -1)
            second_level_new[i][2] = array_for_dislocate2[count++];
}

void MakeRand(int arr[], int count)
{
    srand((unsigned int)time(NULL)); // 随机数种子;
    for (int i = 0; i < count - 1; i++)
    {
        int num = i + rand() % (count - 1 - i); // 取随机数
        int temp = arr[i];
        arr[i] = arr[num];
        arr[num] = temp; // 交换
    }
    int temp = arr[count - 1];
    arr[count - 1] = arr[0];
    arr[0] = temp; // 交换头尾彻底打乱
}

int JudgeSpace()
{
    for (int i = 0; i < 3; i++)
        if (array_for_more_space_new[i] != -1)
            return 0;
    return 1;
}

void MakeExtraSpace()
{
    if (level != 4)
    {
        for (int i = 0; i < 3; i++)
            array_for_more_space_new[i] = store_new[i];
    }
    else
    {
        for (int i = 0; i < 3; i++)
            array_for_resurrect_new[i] = store_new[i];
    }
    for (int i = 3; i < 10; i++)
        store_new[i - 3] = store_new[i];
}

void PrintResurrectSpace()
{
    for (int i = 0; i < 3; i++)
    {
        if (array_for_resurrect_new[i] == -1)
            continue;
        NoteRect.x = 326 + 45 * i;
        NoteRect.y = 550;
        NoteRect.w = NoteSurface2[array_for_resurrect_new[i]]->w + 5;
        NoteRect.h = NoteSurface2[array_for_resurrect_new[i]]->h + 5;
        SDL_Texture *NoteTexture = SDL_CreateTextureFromSurface(Renderer, NoteSurface2[array_for_resurrect_new[i]]);
        SDL_RenderCopy(Renderer, NoteTexture, NULL, &NoteRect);
        SDL_DestroyTexture(NoteTexture);
    }
}

void PrintExtraSpace()
{
    for (int i = 0; i < 3; i++)
    {
        if (array_for_more_space_new[i] == -1)
            continue;
        if (level == 1)
        {
            NoteRect.x = 210 + 65 * i;
            NoteRect.y = 540;
            NoteRect.w = NoteSurface[array_for_more_space_new[i]]->w + 5;
            NoteRect.h = NoteSurface[array_for_more_space_new[i]]->h + 5;
            SDL_Texture *NoteTexture = SDL_CreateTextureFromSurface(Renderer, NoteSurface[array_for_more_space_new[i]]);
            SDL_RenderCopy(Renderer, NoteTexture, NULL, &NoteRect);
            SDL_DestroyTexture(NoteTexture);
        }
        if (level == 3)
        {
            NoteRect.x = 145 + 45 * i;
            NoteRect.y = 550;
            NoteRect.w = NoteSurface2[array_for_more_space_new[i]]->w + 5;
            NoteRect.h = NoteSurface2[array_for_more_space_new[i]]->h + 5;
            SDL_Texture *NoteTexture = SDL_CreateTextureFromSurface(Renderer, NoteSurface2[array_for_more_space_new[i]]);
            SDL_RenderCopy(Renderer, NoteTexture, NULL, &NoteRect);
            SDL_DestroyTexture(NoteTexture);
        }
    }
}