#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "common.h"
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
int main()
{
    int clntSocket = socket(AF_INET, SOCK_STREAM, 0);
    
    sockaddr_in srvAddr;
    memset(&srvAddr, 0, sizeof(srvAddr));
    
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_port = htons(8888);
    srvAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(clntSocket, (sockaddr*)&srvAddr, sizeof(srvAddr)))//connect return 0 sucess
        PERROR_EXIT("connect failed");

    NetPack sendbuf;
    NetPack recvbuf;
    memset(&sendbuf, 0, sizeof(sendbuf));
    memset(&recvbuf, 0, sizeof(recvbuf));

    while (std::cin >> sendbuf.buffer)
    {
        sendbuf.len = htonl(strlen(sendbuf.buffer) + 1);
        ssize_t len = send(clntSocket, &sendbuf, sizeof(int) + strlen(sendbuf.buffer) + 1, 0);
   	    if (len != sizeof(int) + strlen(sendbuf.buffer) + 1)
        	PERROR_EXIT("send failed");

    	len = recv(clntSocket, &recvbuf.len, sizeof(int), 0);
    	if (len < 0)
        	PERROR_EXIT("first recv failed");
        else if (len < sizeof(int))
        {
            std::cout << "first server disconnected(" << len << ")" << std::endl;
            break;
        }

        len = recv(clntSocket, recvbuf.buffer, recvbuf.len, 0);
        if (len < 0)
            PERROR_EXIT("second recv failed");
        else if (len < recvbuf.len)
        {
            std::cout << "second server disconnected(" << len << ")" << std::endl;
            break;
        }

    	std::cout << "echo :" << recvbuf.buffer << std::endl;
        memset(&sendbuf, 0, sizeof(sendbuf));
        memset(&recvbuf, 0, sizeof(recvbuf));
    } 

    close(clntSocket);
    return 0;
}
