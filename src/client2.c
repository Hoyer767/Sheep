#include "chess.h"
#include "client2.h"

#undef main

int main(int argc, char *argv[])
{
    WSADATA wsadata;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (iResult != 0)
    {
        printf("WSAStartup failed :%d\n", iResult);
        Quit();
    }

    // get information
    const char *NAME = "127.0.0.1"; // ip (hostname)(连接到自己这台机器)
    const char *SERVICE = "8080";   // port number
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;

    struct addrinfo *server_addr;
    if (getaddrinfo(NAME, SERVICE, &hints, &server_addr) != 0)
    {
        fprintf(stderr, "get information Usage: failed\n ");
        Quit();
    }

    // create a socket
    socket_server = socket(server_addr->ai_family, server_addr->ai_socktype, server_addr->ai_protocol);
    if (socket_server < 0)
    {
        fprintf(stderr, "create Usage: failed\n ");
        Quit();
    }

    // connect
    if (connect(socket_server, server_addr->ai_addr, server_addr->ai_addrlen) != 0)
    {
        fprintf(stderr, " Usage:connect failed\n ");
        Quit();
    }
    freeaddrinfo(server_addr);

    char judge[3] = "bla";
    send(socket_server, judge, 3, 0);

    printf("connection established\n");

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO); // 初始化
    Mix_Init(MIX_INIT_MP3);
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
    Window = SDL_CreateWindow("chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 850, 850, SDL_WINDOW_SHOWN); /*创建窗口*/
    Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED); /*创建画笔*/                               /*定义主事件*/

    Load(); // 加载图片和文件
    Mix_PlayMusic(Bgm, -1);

    SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);

    Play();

    closesocket(socket_server);
    QUIT();

    return 0;
}

void Load()
{
    MainBackGroundSurface = IMG_Load("pictures/mainbackground.png");
    ReadySurface = IMG_Load("pictures/waiting.png");          // 一个人准备
    Already_ReadySurface = IMG_Load("pictures/starting.png"); // 两个人准备
    FrameSurface = IMG_Load("pictures/frame.png");
    HumoSurface = IMG_Load("pictures/frame.png");
    RegretSurface = IMG_Load("pictures/regret.png");
    for (int i = 1; i <= 24; i++)
    {
        char BoxName[20];
        sprintf(BoxName, "pictures/picture%d.png", i);
        NoteSurface[i - 1] = IMG_Load(BoxName);
    }
    for (int i = 1; i <= 2; i++)
    {
        char BoxName[20];
        sprintf(BoxName, "pictures/win%d.png", i);
        WinSurface[i - 1] = IMG_Load(BoxName);
    }
}

void QUIT()
{
    SDL_FreeSurface(MainBackGroundSurface);
    SDL_FreeSurface(ReadySurface);
    SDL_FreeSurface(Already_ReadySurface);
    SDL_FreeSurface(HumoSurface);
    SDL_FreeSurface(FrameSurface);
    SDL_FreeSurface(RegretSurface);
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);
    Mix_FreeMusic(Bgm);
    for (int i = 0; i < 24; i++)
        SDL_FreeSurface(NoteSurface[i]);
    for (int i = 0; i < 2; i++)
        SDL_FreeSurface(WinSurface[i]);
    Mix_Quit();
    SDL_Quit();
}

void Paint_Chessboard()
{
    SDL_Texture *MainBackGroundTexture = SDL_CreateTextureFromSurface(Renderer, MainBackGroundSurface);
    SDL_RenderCopy(Renderer, MainBackGroundTexture, NULL, NULL);
    SDL_DestroyTexture(MainBackGroundTexture);
}

void Paint_Ready()
{
    NoteRect.x = 320;
    NoteRect.y = 350;
    NoteRect.w = ReadySurface->w + 5;
    NoteRect.h = ReadySurface->h + 5;
    SDL_Texture *ReadyTexture = SDL_CreateTextureFromSurface(Renderer, ReadySurface);
    SDL_RenderCopy(Renderer, ReadyTexture, NULL, &NoteRect);
    SDL_DestroyTexture(ReadyTexture);
}

int Play()
{
    printf("PlayEvent\n");
    while (1)
    {
        PrintAll();
        while (SDL_PollEvent(&PlayEvent))
        {
            fd_set fds;
            FD_ZERO(&fds);
            FD_SET(socket_server, &fds); // 监视socket_server
            fd_set read_fds = fds;
            struct timeval timeout;
            timeout.tv_sec = 0;
            timeout.tv_usec = 0;
            switch (PlayEvent.type)
            {
            case SDL_QUIT:
                QUIT();
                closesocket(socket_server);
                return 0;
            case SDL_MOUSEBUTTONUP:
                printf("(%d,%d)\n", PlayEvent.button.x, PlayEvent.button.y);
                if (start)
                {
                    if (PlayEvent.button.x > 96 && PlayEvent.button.x < 768 && PlayEvent.button.y > 56 && PlayEvent.button.y < 799 && turn && !lose && !whether_replying)
                    {
                        whether_get_power = 0;
                        itoa(PlayEvent.button.x, x_new_black, 10);                 // 十进制（数字转字符串)
                        itoa(PlayEvent.button.y, y_new_black, 10);                 // 十进制（数字转字符串)
                        int bytes_send_x = send(socket_server, x_new_black, 3, 0); // 把字符串放到socket_server里
                        int bytes_send_y = send(socket_server, y_new_black, 3, 0); // 把字符串放到socket_server里
                        if (bytes_send_x == -1 || bytes_send_y == -1)
                        {
                            fprintf(stderr, " Usage:send failed\n ");
                            Quit();
                        }
                        printf("Send %d bytes\n", bytes_send_x);
                        printf("Send %d bytes\n", bytes_send_y);
                    }
                    if (PlayEvent.button.x > 11 && PlayEvent.button.x < 45 && PlayEvent.button.y > 635 && PlayEvent.button.y < 697 && turn && !lose && !whether_replying) // 护摩之杖的点击事件
                    {
                        whether_get_power = 1;
                        x_new_black[0] = 'u';
                        y_new_black[0] = 'u';
                        int bytes_send_x = send(socket_server, x_new_black, 3, 0); // 把字符串放到socket_server里
                        int bytes_send_y = send(socket_server, y_new_black, 3, 0); // 把字符串放到socket_server里
                        if (bytes_send_x == -1 || bytes_send_y == -1)
                        {
                            fprintf(stderr, " Usage:send failed\n ");
                            Quit();
                        }
                    }
                    if (PlayEvent.button.x > 14 && PlayEvent.button.x < 51 && PlayEvent.button.y > 719 && PlayEvent.button.y < 768 && !turn && !lose) // 悔棋的点击事件
                    {
                        x_new_black[0] = 'a';
                        y_new_black[0] = 'a';
                        int bytes_send_x = send(socket_server, x_new_black, 3, 0); // 把字符串放到socket_server里
                        int bytes_send_y = send(socket_server, y_new_black, 3, 0); // 把字符串放到socket_server里
                        if (bytes_send_x == -1 || bytes_send_y == -1)
                        {
                            fprintf(stderr, " Usage:send failed\n ");
                            Quit();
                        }
                    }
                    if (PlayEvent.button.x > 355 && PlayEvent.button.x < 551 && PlayEvent.button.y > 423 && PlayEvent.button.y < 469 && whether_replying && !lose) // 悔棋的点击事件
                    {
                        whether_replying = 0;
                        x_new_black[0] = 'y';
                        y_new_black[0] = 'y';
                        int bytes_send_x = send(socket_server, x_new_black, 3, 0); // 把字符串放到socket_server里
                        int bytes_send_y = send(socket_server, y_new_black, 3, 0); // 把字符串放到socket_server里
                        if (bytes_send_x == -1 || bytes_send_y == -1)
                        {
                            fprintf(stderr, " Usage:send failed\n ");
                            Quit();
                        }
                        for (int i = 0; i < 10; i++)
                            for (int j = 0; j < 9; j++)
                                chessboard_black_new[i][j] = chessboard_black_old[i][j];
                        turn = 0;
                    }                                                                                                                                              // 给个机会
                    if (PlayEvent.button.x > 354 && PlayEvent.button.x < 551 && PlayEvent.button.y > 528 && PlayEvent.button.y < 561 && whether_replying && !lose) // 悔棋的点击事件
                    {
                        whether_replying = 0;
                        x_new_black[0] = 'n';
                        y_new_black[0] = 'n';
                        int bytes_send_x = send(socket_server, x_new_black, 3, 0); // 把字符串放到socket_server里
                        int bytes_send_y = send(socket_server, y_new_black, 3, 0); // 把字符串放到socket_server里
                        if (bytes_send_x == -1 || bytes_send_y == -1)
                        {
                            fprintf(stderr, " Usage:send failed\n ");
                            Quit();
                        }
                    }                                                                                                                         // 残忍拒绝
                    if (PlayEvent.button.x > 335 && PlayEvent.button.x < 531 && PlayEvent.button.y > 475 && PlayEvent.button.y < 521 && lose) // 结束时的点击事件
                    {
                        self_ready = 1;
                        x_new_black[0] = 'f';
                        y_new_black[0] = 'f';
                        int bytes_send_x = send(socket_server, x_new_black, 3, 0); // 把字符串放到socket_server里
                        int bytes_send_y = send(socket_server, y_new_black, 3, 0); // 把字符串放到socket_server里
                        if (bytes_send_x == -1 || bytes_send_y == -1)
                        {
                            fprintf(stderr, " Usage:send failed\n ");
                            Quit();
                        }
                    }
                    switch (Judge(transfer_x(x_old_black), transfer_y(y_old_black), transfer_x(x_new_black), transfer_y(y_new_black), chessboard_black_new, 2))
                    {
                    case 1:
                        turn = 0;
                        black = 1;
                        Store(chessboard_black_new, chessboard_black_old);
                        int tempt = chessboard_black_new[transfer_y(y_old_black)][transfer_x(x_old_black)];
                        chessboard_black_new[transfer_y(y_old_black)][transfer_x(x_old_black)] = 0;
                        // 动画效果
                        for (int i = 0; i < 20; i++)
                        {
                            NoteRect.x = 84 + 78 * transfer_x(x_old_black) + i * (float)((78 * (transfer_x(x_new_black) - transfer_x(x_old_black))) / 20);
                            NoteRect.y = 47 + 78 * transfer_y(y_old_black) + i * (float)((78 * (transfer_y(y_new_black) - transfer_y(y_old_black))) / 20);
                            NoteRect.w = FrameSurface->w + 5;
                            NoteRect.h = FrameSurface->h + 5;
                            SDL_Texture *ChessPieceTexture = SDL_CreateTextureFromSurface(Renderer, NoteSurface[tempt - 1]);
                            SDL_RenderCopy(Renderer, ChessPieceTexture, NULL, &NoteRect);
                            SDL_DestroyTexture(ChessPieceTexture);
                            SDL_RenderPresent(Renderer);
                            SDL_RenderClear(Renderer);
                            Paint_Chessboard();
                            PaintTheChessPieces();
                        }
                        // 存储新棋盘
                        chessboard_black_new[transfer_y(y_new_black)][transfer_x(x_new_black)] = (tempt % 2 || !(tempt % 12)) ? tempt : tempt - 1;
                        for (int i = 0; i < 3; i++)
                        {
                            x_new_black[i] = '\0';
                            x_old_black[i] = '\0';
                            y_new_black[i] = '\0';
                            y_old_black[i] = '\0';
                        }
                        switch (JudgeWinOrLose(chessboard_black_new))
                        {
                        case 0:
                            lose = 0;
                            break;
                        case 1:
                            lose = 2;
                            break;
                        case 2:
                            lose = 1;
                            break;
                        default:
                            break;
                        } // 判断输赢
                        break;
                    case 2:
                        black = 1;
                        whether_get_power = 0;
                        turn = 0;
                        Store(chessboard_black_new, chessboard_black_old);
                        chessboard_black_new[transfer_y(y_new_black)][transfer_x(x_new_black)] += (chessboard_black_new[transfer_y(y_new_black)][transfer_x(x_new_black)] % 2) ? 1 : -1;
                        for (int i = 0; i < 3; i++)
                        {
                            x_new_black[i] = '\0';
                            x_old_black[i] = '\0';
                            y_new_black[i] = '\0';
                            y_old_black[i] = '\0';
                        }
                        break;
                    default:
                        break;
                    }
                    for (int i = 0; i < 3; i++)
                    {
                        x_old_black[i] = x_new_black[i];
                        y_old_black[i] = y_new_black[i];
                    } // 更新数据
                }
                wait = 1;
                break;
            default:
                break;
            }
            if (select(socket_server + 1, &read_fds, NULL, NULL, &timeout) == -1)
            {
                fprintf(stderr, "select() failed\n");
                printf("%d\n", WSAGetLastError());
                break;
            }
            if (FD_ISSET(socket_server, &read_fds))
            {
                if (lose)
                {
                    opp_ready = 1;
                }
                if (start && turn && !lose)
                {
                    char tempt1[4], tempt2[4];
                    int bytes_received = which_to_receive ? recv(socket_server, tempt1, 3, 0) : recv(socket_server, tempt2, 3, 0);
                    if (black)
                        whether_replying = 1;
                }
                if (!start && !lose)
                {
                    start = 1;
                }
                if (start && !turn && !lose)
                {
                    int bytes_received = which_to_receive ? recv(socket_server, y_new_red, 3, 0) : recv(socket_server, x_new_red, 3, 0);
                    if (bytes_received < 1)
                    {
                        printf("%d\n", WSAGetLastError());
                        fprintf(stderr, " Usage:receive failed\n ");
                        FD_CLR(socket_server, &fds);
                        closesocket(socket_server);
                        Quit();
                    }
                    printf("Received %d bytes from server: %.*s\n", bytes_received, bytes_received, which_to_receive ? y_new_red : x_new_red);
                    which_to_receive = which_to_receive ? 0 : 1;
                    if (!which_to_receive)
                    {
                        switch (Judge(8 - transfer_x(x_old_red), 9 - transfer_y(y_old_red), 8 - transfer_x(x_new_red), 9 - transfer_y(y_new_red), chessboard_black_new, 3))
                        {
                        case 1:
                            turn = 1;
                            Store(chessboard_black_new, chessboard_black_old);
                            int tempt = chessboard_black_new[9 - transfer_y(y_old_red)][8 - transfer_x(x_old_red)];
                            chessboard_black_new[9 - transfer_y(y_old_red)][8 - transfer_x(x_old_red)] = 0;
                            // 动画效果
                            for (int i = 0; i < 20; i++)
                            {
                                NoteRect.x = 84 + 78 * (8 - transfer_x(x_old_red)) + i * (float)((78 * (transfer_x(x_old_red) - transfer_x(x_new_red))) / 20);
                                NoteRect.y = 47 + 78 * (9 - transfer_y(y_old_red)) + i * (float)((78 * (transfer_y(y_old_red) - transfer_y(y_new_red))) / 20);
                                NoteRect.w = FrameSurface->w + 5;
                                NoteRect.h = FrameSurface->h + 5;
                                SDL_Texture *ChessPieceTexture = SDL_CreateTextureFromSurface(Renderer, NoteSurface[tempt - 1]);
                                SDL_RenderCopy(Renderer, ChessPieceTexture, NULL, &NoteRect);
                                SDL_DestroyTexture(ChessPieceTexture);
                                SDL_RenderPresent(Renderer);
                                SDL_RenderClear(Renderer);
                                Paint_Chessboard();
                                PaintTheChessPieces();
                            }
                            // 存储新棋盘
                            chessboard_black_new[9 - transfer_y(y_new_red)][8 - transfer_x(x_new_red)] = (tempt % 2 || !(tempt % 12)) ? tempt : tempt - 1;
                            for (int i = 0; i < 3; i++)
                            {
                                x_new_red[i] = '\0';
                                x_old_red[i] = '\0';
                                y_new_red[i] = '\0';
                                y_old_red[i] = '\0';
                            }
                            switch (JudgeWinOrLose(chessboard_black_new))
                            {
                            case 0:
                                lose = 0;
                                break;
                            case 1:
                                lose = 2;
                                break;
                            case 2:
                                lose = 1;
                                break;
                            default:
                                break;
                            } // 判断输赢
                            break;
                        case 2:
                            turn = 1;
                            Store(chessboard_black_new, chessboard_black_old);
                            chessboard_black_new[9 - transfer_y(y_new_red)][8 - transfer_x(x_new_red)] += (chessboard_black_new[9 - transfer_y(y_new_red)][8 - transfer_x(x_new_red)] % 2) ? 1 : -1;
                            for (int i = 0; i < 3; i++)
                            {
                                x_new_red[i] = '\0';
                                x_old_red[i] = '\0';
                                y_new_red[i] = '\0';
                                y_old_red[i] = '\0';
                            }
                            break;
                        case 3:
                            printf("here\n");
                            for (int i = 0; i < 10; i++)
                                for (int j = 0; j < 9; j++)
                                    chessboard_black_new[i][j] = chessboard_black_old[i][j];
                            turn = 1;
                            break;
                        case 4:
                            break;
                        default:
                            break;
                        }
                        for (int i = 0; i < 3; i++)
                        {
                            x_old_red[i] = x_new_red[i];
                            y_old_red[i] = y_new_red[i];
                        } // 更新数据
                    }     // 如果黑棋触发了合法操作，就更换下棋方并改变棋盘参数
                }
            }
        }
        if (self_ready && opp_ready)
            Again();
    }
}
void Store(int chessboard_new[][9], int chessboard_old[][9])
{
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 9; j++)
            chessboard_old[i][j] = chessboard_new[i][j];
}

void PrintAll()
{
    Paint_Chessboard();
    PaintTheChessPieces();
    if (start && !wait)
        Paint_Already_Ready();
    if ((!start || self_ready) && !(self_ready && opp_ready))
        Paint_Ready();
    if (12 < chessboard_black_new[transfer_y(y_new_black)][transfer_x(x_new_black)] && chessboard_black_new[transfer_y(y_new_black)][transfer_x(x_new_black)] < 25)
        PaintTheFrame();
    if (whether_get_power)
        PaintHumo();
    if (lose && !self_ready)
        PaintWinOrLose();
    if (whether_replying)
        PaintRegret();
    SDL_RenderPresent(Renderer);
}

void PaintRegret()
{
    NoteRect.x = 250;
    NoteRect.y = 250;
    NoteRect.w = RegretSurface->w + 5;
    NoteRect.h = RegretSurface->h + 5;
    SDL_Texture *RegretTexture = SDL_CreateTextureFromSurface(Renderer, RegretSurface);
    SDL_RenderCopy(Renderer, RegretTexture, NULL, &NoteRect);
    SDL_DestroyTexture(RegretTexture);
}

void PaintTheFrame()
{
    NoteRect.x = 84 + 78 * transfer_x(x_new_black);
    NoteRect.y = 47 + 78 * transfer_y(y_new_black);
    NoteRect.w = FrameSurface->w + 5;
    NoteRect.h = FrameSurface->h + 5;
    SDL_Texture *FrameTexture = SDL_CreateTextureFromSurface(Renderer, FrameSurface);
    SDL_RenderCopy(Renderer, FrameTexture, NULL, &NoteRect);
    SDL_DestroyTexture(FrameTexture);
}

void Paint_Already_Ready()
{
    NoteRect.x = 320;
    NoteRect.y = 350;
    NoteRect.w = Already_ReadySurface->w + 5;
    NoteRect.h = Already_ReadySurface->h + 5;
    SDL_Texture *ReadyTexture = SDL_CreateTextureFromSurface(Renderer, Already_ReadySurface);
    SDL_RenderCopy(Renderer, ReadyTexture, NULL, &NoteRect);
    SDL_DestroyTexture(ReadyTexture);
    SDL_RenderPresent(Renderer);
    wait = 1;
    Sleep(1500);
}

void PaintTheChessPieces()
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (chessboard_black_new[i][j] == 0)
                continue;
            else
            {
                NoteRect.x = 114 + j * 78 - 30;
                NoteRect.y = 77 + i * 78 - 30;
                NoteRect.w = NoteSurface[chessboard_black_new[i][j] - 1]->w + 6;
                NoteRect.h = NoteSurface[chessboard_black_new[i][j] - 1]->h + 6;
                SDL_Texture *ChessPieceTexture = SDL_CreateTextureFromSurface(Renderer, NoteSurface[chessboard_black_new[i][j] - 1]);
                SDL_RenderCopy(Renderer, ChessPieceTexture, NULL, &NoteRect);
                SDL_DestroyTexture(ChessPieceTexture);
            }
        }
    }
}

void PaintHumo()
{
    NoteRect.x = 3;
    NoteRect.y = 637;
    NoteRect.w = FrameSurface->w + 5;
    NoteRect.h = FrameSurface->h + 5;
    SDL_Texture *HumoTexture = SDL_CreateTextureFromSurface(Renderer, HumoSurface);
    SDL_RenderCopy(Renderer, HumoTexture, NULL, &NoteRect);
    SDL_DestroyTexture(HumoTexture);
}

void PaintWinOrLose()
{
    NoteRect.x = 300;
    NoteRect.y = 300;
    NoteRect.w = ((lose == 1) ? WinSurface[0] : WinSurface[1])->w + 5;
    NoteRect.h = ((lose == 1) ? WinSurface[0] : WinSurface[1])->h + 5;
    SDL_Texture *WinOrLoseTexture = SDL_CreateTextureFromSurface(Renderer, (lose == 1) ? WinSurface[0] : WinSurface[1]);
    SDL_RenderCopy(Renderer, WinOrLoseTexture, NULL, &NoteRect);
    SDL_DestroyTexture(WinOrLoseTexture);
}

void Again()
{
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 9; j++)
        {
            chessboard_black_new[i][j] = chessboard_black_copy[i][j];
            chessboard_black_old[i][j] = chessboard_black_copy[i][j];
        }
    whether_get_power = 0;
    lose = 0;             // 0代表还在对战中，1代表输了，2代表赢了
    whether_replying = 0; // 是否收到了悔棋申请
    black = 0;
    self_ready = 0;
    opp_ready = 0;
}