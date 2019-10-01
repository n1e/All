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

        char szBuffer[2048] = {0};
        ssize_t len = recv(clntSocket, szBuffer, sizeof(szBuffer), 0);
        if (len < 0)
            PERROR_EXIT("recv failed");

        while (len > 0)
        {
            ssize_t send_len = send(clntSocket, szBuffer, len, 0);
            if (send_len != len)
                PERROR_EXIT("send error");
            
            if ((len = recv(clntSocket, szBuffer, sizeof(szBuffer), 0))  < 0)
                PERROR_EXIT("recv failed");
        }
        
        close(clntSocket);
    }
}
 
