#include "chess.h"
#include "server.h"

int main(int argc, char *argv[]) // 控制主界面
{
    WSADATA wsadata;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (iResult != 0)
    {
        printf("WSAStartup failed :%d\n", iResult);
        Quit();
    }

    /* 1. getaddrinfo */
    const char *NAME = NULL;      // ip (hostname)
    const char *SERVICE = "8080"; // port number

    struct addrinfo *bind_addr, hints;

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_INET;                               // IPv4
    hints.ai_socktype = SOCK_STREAM;                         // TCP
    hints.ai_flags = AI_PASSIVE;                             // use "0.0.0.0" 便于监听，only for bind
                                                             // 标准提示 筛选出有必要的ip地址（服务器端）（因为有多个）
    if (getaddrinfo(NAME, SERVICE, &hints, &bind_addr) != 0) // getaddrinfo函数会修改bind_addr，将其指向符合条件的存有ip信息链表的头部
    {
        printf("%d\n", getaddrinfo);
        fprintf(stderr, "getaddrinfo() failed\n"); // 返回值存储ip地址的链表的头指针
        Quit();
    }

    // 2. create socket//socket 可以理解为港口信息牌
    int socket_listen = socket(bind_addr->ai_family, bind_addr->ai_socktype, bind_addr->ai_protocol);
    if (socket_listen == -1)
    {
        fprintf(stderr, "socket() failed\n");
        Quit();
    }

    // 3.bind
    if (bind(socket_listen, bind_addr->ai_addr, bind_addr->ai_addrlen) != 0)
    {
        fprintf(stderr, "bind() failed\n");
        Quit();
    }

    freeaddrinfo(bind_addr);

    // 4.listen
    if (listen(socket_listen, 32) != 0)
    {
        fprintf(stderr, "listen failed\n");
        Quit();
    } // 32 -- 最多有多少个等待者

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(socket_listen, &fds);
    int max_socket = socket_listen;

    SOCKET socket_client[2] = {INVALID_SOCKET};

    printf("Waiting for connection\n");

    while (1)
    {
        fd_set reads = fds; // 创建一个副本 select函数会改变reads中的值
        if (select(max_socket + 1, &reads, NULL, NULL, NULL) < 0)
        {
            printf("select() failed\n");
            break;
        } // 开始选择

        for (int i = 1; i <= max_socket; i++)
        {
            if (FD_ISSET(i, &reads))
            {
                if (i == socket_listen) // 其实就是有新的端口接入了
                {
                    socket_client[times] = accept(socket_listen, NULL, NULL); // 如果连接成功会创建另一个socket_client;
                    if (socket_client[times] < 0)
                    {
                        fprintf(stderr, "accept failed\n");
                        Quit();
                    }

                    FD_SET(socket_client[times], &fds); // 将自己创立的socket_client加入监听范围，用来监听并接受客户端发出的信息
                    if (socket_client[times] > max_socket)
                        max_socket = socket_client[times];
                    times++;
                    if (times == 2)
                    {
                        for (int i = 0; i < 2; i++)
                        {
                            send(socket_client[i], starting, 3, 0);
                            send(socket_client[i], starting, 3, 0);
                        }
                        start = 1;
                        if (change)
                        {
                            SOCKET tempt;
                            tempt = socket_client[0];
                            socket_client[0] = socket_client[1];
                            socket_client[1] = tempt;
                            printf("changed\n");
                        } // 交换红黑socket位置，这样无论如何红方都是socket[0];
                    }     // 两个人都准备好的话就可以开始了
                }
                // accept the new socket
                else
                { // i == socket_client[0/1] 有客户端发送信息过来了
                    if (i == socket_client[0] && start)
                    {
                        int bytes_received = which_to_receive_red ? recv(i, y_new_red, 3, 0) : recv(i, x_new_red, 3, 0);
                        if (bytes_received < 1)
                        {
                            printf("%d\n", WSAGetLastError());
                            fprintf(stderr, " Usage:receive failed\n ");
                            FD_CLR(i, &fds);
                            closesocket(i);
                            closesocket(socket_listen);
                            Quit();
                        }
                        printf("Received %d bytes from red: %.*s\n", bytes_received, bytes_received, which_to_receive_red ? y_new_red : x_new_red);
                        if (x_new_red[0] != 'b' && x_new_red[0] != 'r')
                            which_to_receive_red = which_to_receive_red ? 0 : 1;
                        if (!which_to_receive_red)
                        {
                            int bytes_send_x = send(socket_client[1], x_new_red, 3, 0); // 把信息发送给黑方
                            int bytes_send_y = send(socket_client[1], y_new_red, 3, 0); // 把信息发送给黑方
                            for (int i = 0; i < 3; i++)
                            {
                                x_old_red[i] = x_new_red[i];
                                y_old_red[i] = y_new_red[i];
                            }
                        }
                    } // 红方点击事件
                    if (i == socket_client[1] && start)
                    {
                        int bytes_received = which_to_receive_black ? recv(i, y_new_black, 3, 0) : recv(i, x_new_black, 3, 0);
                        if (bytes_received < 1)
                        {
                            printf("%d\n", WSAGetLastError());
                            fprintf(stderr, " Usage:receive failed\n ");
                            FD_CLR(i, &fds);
                            closesocket(i);
                            closesocket(socket_listen);
                            Quit();
                        }
                        printf("Received %d bytes from black: %.*s\n", bytes_received, bytes_received, which_to_receive_black ? y_new_black : x_new_black);
                        printf("%d\n", which_to_receive_black);
                        if (x_new_black[0] != 'b' && x_new_black[0] != 'r')
                            which_to_receive_black = which_to_receive_black ? 0 : 1;
                        if (!which_to_receive_black)
                        {
                            int bytes_send_x = send(socket_client[0], x_new_black, 3, 0); // 把信息发送给红方
                            int bytes_send_y = send(socket_client[0], y_new_black, 3, 0); // 把信息发送给红方
                            for (int i = 0; i < 3; i++)
                            {
                                x_old_black[i] = x_new_black[i];
                                y_old_black[i] = y_new_black[i];
                            }
                        }
                    }
                } // 黑方点击事件
                if (!start)
                {
                    char judge[3] = "ooo";
                    int bytes = recv(i, judge, 3, 0);
                    if (judge[0] == 'b') // 黑方先进来了
                    {
                        change = 1;
                        printf("black enter before the red\n");
                    } // 需要改变一下黑红socket
                }
            }
        }
    }
    printf("%d\n", WSAGetLastError());
    closesocket(socket_listen);
    Quit();
    return 0;
}
