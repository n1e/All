#include "../simple server/common.h"
int domain(int sock)
{
    fd_set readSet;
    FD_ZERO(&readSet);

    int fd_stdin = fileno(std::cin);
    timeval timeout;

    char readBuffer[2048] = {0};
    char writeBuffer[2048] = {0};

    while(1)
    {   
        timeout.tv_sec = 2;
        timeout.tv_usec = 0;
        FD_SET(sock, &readSet);
        FD_SET(fd_stdin, &readSet);
        
        int maxfd = sock > fd_stdin ? sock : fd_stdin;
        int nCount = select(maxfd + 1, &readSet, NULL, NULL, &timeout);
        if (nCount == -1)
            PERROR_EXIT("select failed");
        else if (nCount == 0)
            continue;
        
        if (FD_ISSET(sock, &readSet))
        {
            int nLen = readline(sock, readBuffer, sizeof(readBuffer));
            if (nLen < 0)
                PERROR_EXIT("readline failed");
            else if (nLen == 0)
            {
                std::cout << "server closed" << std::endl;
                break;
            }

            std::cout << "echo: " << readBuffer;
            memset(readBuffer, 0, sizeof(readBuffer));
        }

        if (FD_ISSET(fd_stdin, &readSet))
        {
            if (std::cin >> writeBuffer)
                writen(sock, writeBuffer, strlen(writeBuffer));
            
            memset(writeBuffer, 0, sizeof(writeBuffer));
        }
    }
    close(sock);
}
int main()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        PERROR_EXIT("create socket failed");
    
    sockaddr_in addrSrv;
    memset(&addrSrv, 0, sizeof(sockaddr_in));
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_addr.s_addr = inet_addr("127.0.0.1");
    addrSrv.sin_port = htons(8888);

    if (connect(sock, (sockaddr*)&addrSrv, sizeof(sockaddr_in)))
        PERROR_EXIT("connect failed");

    domain(sock);

    return 0;
}