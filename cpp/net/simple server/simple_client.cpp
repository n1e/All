#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "common.h"
#include <sys/socket.h>


int main()
{
    int clntSocket = socket(AF_INET, SOCK_STREAM, 0);
    
    sockaddr_in srvAddr;
    memset(&srvAddr, 0, sizeof(srvAddr));
    
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_port = htons(8888);
    srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (!connect(clntSocket, (sockaddr*)&srvAddr, sizeof(srvAddr))
        PERROR_EXIT("connect failed");

    char buffer[2048] = "hello world";
    ssize_t len = send(clntSocket, buffer, strlen(buffer) + 1, 0);
    if (len != strlen(buffer))
        PERROR_EXIT("send failed");

    len = recv(clntSocket, buffer, sizeof(buffer), 0);
    if (len < 0)
        PERROR_EXIT("recv failed");

    std::cout << "echo :" << buffer << endl;

    close(clntSocket);
}