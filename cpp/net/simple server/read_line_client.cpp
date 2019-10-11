#include "common.h"

int main()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        PERROR_EXIT("create socket failed");

    sockaddr_in srvaddr;
    memset(&srvaddr, 0, sizeof(srvaddr));
    srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_port = htons(8888);
    
    if (connect(sock, (sockaddr*)&srvaddr, sizeof(srvaddr)))
        PERROR_EXIT("connect failed");
    
    char szbuff[2048];
    char szMsg[2048];
    memset(szbuff, 0, sizeof(szbuff));
    memset(szMsg, 0, sizeof(szMsg));
    while (std::cin >> szbuff)
    {
        int nlen = strlen(szbuff);
        szbuff[nlen] = '\n';
        int ret = write(sock, szbuff, nlen + 1);
        if (ret < 0)
            PERROR_EXIT("writen failed");
        
        ret = readline(sock, szMsg, sizeof(szMsg));
        if (ret < 0)
            PERROR_EXIT("readn failed");
        else if (ret == 0)
        {
            PERROR_EXIT("readn length not equal need");
            exit(EXIT_SUCCESS);
        }
        
        std::cout << "echo: " << szMsg;
        memset(szbuff, 0, sizeof(szbuff));
        memset(szMsg, 0, sizeof(szMsg));
    }
    close(sock);
    return 0;
}