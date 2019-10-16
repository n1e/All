#include "common.h"

int main()
{
    int sock_list[5] = {0};
    for (auto &i : sock_list)
    {
        i = socket(AF_INET, SOCK_STREAM, 0);
        if (i < 0)
            PERROR_EXIT("create socket failed");
    }
    
    sockaddr_in addr_srv;
    memset(&addr_srv, 0, sizeof(addr_srv));
    addr_srv.sin_family = AF_INET;
    addr_srv.sin_port = htons(8888);
    addr_srv.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    for ( auto sock : sock_list)
    {
        if (connect((int)sock, (sockaddr*)&addr_srv, sizeof(addr_srv)))
            PERROR_EXIT("connect failed");

        static int num = 0;
        char msg[1024] = {0};
        sprintf(msg, "hello, I'm client [%d]\n", ++num);
        int nLen = writen(sock, msg, strlen(msg));
        if (nLen < 0)
            PERROR_EXIT("writen failed");
        
        memset(msg, 0, sizeof(msg));
        int nReadLen = readline(sock, msg, sizeof(msg));
        if (nReadLen < 0)
        {
            PERROR_EXIT("readline failed");
        }
        else if  (nReadLen < nLen)
        {
            std::cout << "server disconnected" << std::endl;
            close(sock);
            exit(EXIT_SUCCESS);
        }

        std::cout << "echo: " << msg;
    }
    
    return 0;
}