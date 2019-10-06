#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "common.h"

void recv_client_msg(const int sock, const char *ipaddr)
{
    NetPack recvbuf;
    while (1)
    {
        memset(&recvbuf, 0, sizeof(recvbuf));
        ssize_t len = recv(sock, &(recvbuf.len), sizeof(int), 0);

        if (len < 0)
        {
            PERROR_EXIT("first recv failed");
        }
        else if (len < sizeof(int))
        {
            close(sock);
            std::cout <<  "client " << ipaddr << " disconnected" << std::endl;
            exit(EXIT_SUCCESS);
        }

        len = recv(sock, recvbuf.buffer, ntohl(recvbuf.len), 0);
        if (len < 0)
        {
            PERROR_EXIT("second recv failed");
        }
        else if (len < ntohl(recvbuf.len))
        {
            close(sock);
            std::cout <<  "client " << ipaddr << " disconnected" << std::endl;
            exit(EXIT_SUCCESS);
        }
        
        std::cout << "client " << ipaddr << ": " << recvbuf.buffer << std::endl;
        
        ssize_t send_len = send(sock, &recvbuf, len + ntohl(recvbuf.len), 0);
        if (send_len != len + ntohl(recvbuf.len))
            PERROR_EXIT("send error");
    }
};

int main()
{
    int srvSocket = socket(AF_INET, SOCK_STREAM, 0);
    if ( srvSocket < 0)
        PERROR_EXIT("create socket failed");
    
    sockaddr_in srvAddr;
    memset(&srvAddr, 0, sizeof(sockaddr_in));
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_port = htons(8888);
    srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int nOn = 1;
    if (setsockopt(srvSocket, SOL_SOCKET, SO_REUSEADDR, (const void*)&nOn, sizeof(nOn)) < 0)
        PERROR_EXIT("set option reuseraddr failed");
        
    if (bind(srvSocket, (sockaddr*)&srvAddr, sizeof(srvAddr)) < 0)
        PERROR_EXIT("bind failed");

    if (listen(srvSocket, 5) < 0)
        PERROR_EXIT("listen error");
    
    while (1)
    {
        sockaddr_in clntAddr;
        socklen_t addr_len = sizeof(sockaddr_in);

        int clntSocket = accept(srvSocket, (sockaddr*)&clntAddr, &addr_len);
        if (clntSocket < 0)
            PERROR_EXIT("accept failed");

        char ip_clnt[255] = {0};
        if (inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr, ip_clnt, sizeof(ip_clnt)) != NULL)
            std::cout << "eastablished connection from " << ip_clnt << std::endl;
        else
            std::cout << "error connection" << std::endl;

        pid_t pid = fork();
        if (pid == -1)
            PERROR_EXIT("fork failed");
        else if (pid == 0)
        {
            close(srvSocket);
            recv_client_msg(clntSocket, ip_clnt);
        }
        else
        {
            close(clntSocket);
        }
        
    }
}
 
