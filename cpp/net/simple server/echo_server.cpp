#include "common.h"
#include <sys/wait.h>
void echo_srv(int sock)
{
    while (1)
    {
        sockaddr_in addr_clnt;
        socklen_t socklen = sizeof(addr_clnt);
        memset(&addr_clnt, 0, sizeof(addr_clnt));
        
        int sock_clnt = accept(sock, (sockaddr*)&sock_clnt, &socklen);
        if (sock_clnt < 0)
            PERROR_EXIT("accept failed");

        int pid = fork();
        if (pid < 0)
            PERROR_EXIT("fork failed");

        if (pid == 0)
        {
            close(sock);
            char buffer[2048] = {0};
            
            while (1)
            {
                int nLen = readline(sock_clnt, buffer, sizeof(buffer));
                if (nLen == 0)
                {
                    close(sock_clnt);
                    std::cout << "client [" << inet_ntoa(addr_clnt.sin_addr) << "] disconnected from server" << std::endl;
                    exit(EXIT_SUCCESS);
                }
                else if (nLen < 0)
                {
                    PERROR_EXIT("readline failed");
                }

                std::cout << "recv [" << nLen << "] bytes data from client [" << inet_ntoa(addr_clnt.sin_addr) << "],content: " << buffer;
                nLen = writen(sock_clnt, buffer, nLen);
                if (nLen < 0)
                    PERROR_EXIT("writen failed");
            }    
        }
        else
        {
            close(sock_clnt);
        }  
    }

}

void handler(int sig)
{
    std::cout << "recv a sig " << sig << std::endl;
    while (waitpid(-1, NULL, WNOHANG) > 0) ;
}

int main()
{
    int sock_srv = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_srv < 0)
        PERROR_EXIT("create socket failed");
    
    sockaddr_in addr_srv;
    memset(&addr_srv, 0, sizeof(addr_srv));
    addr_srv.sin_family = AF_INET;
    addr_srv.sin_port = htons(8888);
    addr_srv.sin_addr.s_addr = htonl(INADDR_ANY);
    
    int opt = 1;
    setsockopt(sock_srv, SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(sock_srv));
    if (bind(sock_srv, (sockaddr*)&addr_srv, sizeof(addr_srv)))
        PERROR_EXIT("bind failed");
    
    if (listen(sock_srv, 256))
        PERROR_EXIT("listen failed");

    signal(SIGCHLD, handler);
    echo_srv(sock_srv);
}