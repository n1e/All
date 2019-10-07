#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "../simple server/common.h"

void handler(int sig)
{
    std::cout << "recv a sig=" << sig << std::endl;
    exit(EXIT_SUCCESS);
}

int main()
{
    int srvsock = 0;
    if ((srvsock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        PERROR_EXIT("create socket failed");

    sockaddr_in srvadddr;
    memset(&srvadddr, 0, sizeof(srvsock));
    srvadddr.sin_family = AF_INET;
    srvadddr.sin_port = htons(8888);
    srvadddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int nReUseAddrOpen = 1;
    if (setsockopt(srvsock, SOL_SOCKET, SO_REUSEADDR, (const void*)&nReUseAddrOpen, sizeof(nReUseAddrOpen)))
        PERROR_EXIT("set reuseaddr opt failded");

    if (bind(srvsock, (sockaddr *)&srvadddr, sizeof(srvadddr)))
        PERROR_EXIT("bind server addr failed");

    if (listen(srvsock, 64))
        PERROR_EXIT("listen failed");

    int count = 0;
    while (1)
    {
        std::cout << "[DEBUG]" << ++count << " 's loop" << std::endl;
        std::cout << "[DEBUG] main pid=" << getpid() << std::endl;

        sockaddr_in clientaddr;
        memset(&clientaddr, 0, sizeof(sockaddr_in));
        socklen_t clientaddr_len = sizeof(sockaddr_in);
        int clientsock = 0;

        if ((clientsock = accept(srvsock, (sockaddr *)&clientaddr, &clientaddr_len)) < 0)
            PERROR_EXIT("accept failed");
        
        char ip_clnt[16] = {0};
        strcpy(ip_clnt, inet_ntoa(clientaddr.sin_addr));
        std::cout << ip_clnt << "connected" << std::endl;

        pid_t pid = 0;
        if ((pid = fork()) < 0)
            PERROR_EXIT("first fork failed");
        
        if (pid == 0)
        {
            signal(SIGUSR1, handler);
            std::cout << "[DEBUG] child pid=" << getpid() << std::endl;
            close(srvsock);
            
            pid_t pid = 0;
            if ((pid = fork()) < 0)
                PERROR_EXIT("second fork failed");
            
            if (pid == 0) // read
            {
                std::cout << "[DEBUG] read pid=" << getpid() << std::endl;
                char buffer[2048] = {0};
                while (1)
                {
                    memset(buffer, 0, sizeof(buffer));
                    int recv_len = recv(clientsock, buffer, sizeof(buffer), 0);
                    if (recv_len < 0)
                        PERROR_EXIT("recv failed");
                    else if (recv_len == 0)
                    {
                        std::cout << ip_clnt << "disconnected" << std::endl;
                        close(clientsock);
                        break;
                    }
                    else
                    {
                        std::cout << ip_clnt << ": " << buffer << std::endl;
                    }
                }
                std::cout << "close server read process" << std::endl;
                std::cout << "getppid=" << getppid() << std::endl;
                kill(getppid(), SIGUSR1);
                exit(EXIT_SUCCESS);
            }
            else // write
            {
                std::cout << "[DEBUG] write pid=" << getpid() << std::endl;
                char msg[2048];
                while (std::cin >> msg)
                {
                    if (strlen(msg) != send(clientsock, msg, strlen(msg), 0))
                        PERROR_EXIT("send failed");

                    memset(msg, 0, sizeof(msg));
                }
                std::cout << "close server write process" << std::endl;
            }

        }
        else
        {
            close(clientsock);
        }
    }
    return 0;
}



