#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "string.h"
#include <unistd.h>
#include <signal.h>
#include "../simple server/common.h"

void handler(int sig)
{
    std::cout << "recv a sig=" << sig << std::endl;
    //exit(EXIT_SUCCESS);
}

int main()
{
    int sock = 0;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        PERROR_EXIT("create socket failed");
    
    sockaddr_in srvaddr;
    memset(&srvaddr, 0, sizeof(srvaddr));
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_port = htons(8888);
    srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (connect(sock, (sockaddr*)&srvaddr, sizeof(srvaddr)))
        PERROR_EXIT("connect failed");


    std::cout << "[DEBUG] main pid=" << getpid() << std::endl;
    pid_t pid = 0;
    if ((pid = fork()) < 0)
        PERROR_EXIT("fork failed");

    if ( pid == 0)
    {
        
        std::cout << "[DEBUG] read pid=" << getpid() << std::endl;
        char buffer[2048];
        while (1)
        {
            memset(buffer, 0, sizeof(buffer));
            int rev_len = recv(sock, buffer, sizeof(buffer), 0);
            if (rev_len < 0)
                PERROR_EXIT("recv failed");
            else if (rev_len == 0)
            {
                close(sock);
                break;
            }
            else
            {
                std::cout << "server :" << buffer << std::endl;
            }
        }
        std::cout << "close client read process" << std::endl;
        std::cout << "getppid=" << getppid() << std::endl;
        kill(getppid(), SIGUSR1);
        exit(EXIT_SUCCESS);
    }
    else
    {
        std::cout << "[DEBUG] write pid=" << getpid() << std::endl;
        signal(SIGUSR1, handler);
        char msg[2048] = {0};
        while (std::cin >> msg)
        {
            int send_len = 0;
            if ((send_len = send(sock, msg, sizeof(msg), 0)) <= 0)
                PERROR_EXIT("send failed");

            memset(msg, 0, sizeof(msg));
        }
        close(sock);
        std::cout << "close client write process" << std::endl;
    }
    return 0;
}