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

    char buffer[4][2048] = {"hello world", "xixihaha", "nice!", "endend"};
 
    for (int i = 0 ; i < 4 ; i++)
    {
    	ssize_t len = send(clntSocket, buffer[i], strlen(buffer[i]) + 1, 0);
   	if (len != strlen(buffer[i]) + 1)
        	PERROR_EXIT("send failed");

    	len = recv(clntSocket, buffer[i], 2048, 0);
    	if (len < 0)
        	PERROR_EXIT("recv failed");

    	std::cout << "echo :" << buffer[i] << std::endl;
    }

    close(clntSocket);
    return 0;
}
