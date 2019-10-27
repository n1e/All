#include "common.h"

void handler(int sig)
{
    std::cout << "recv a sig: " << sig << std::endl << "exit!";
    exit(EXIT_SUCCESS);
};

int main()
{
    int sock = 0;
    if (0 > (sock = socket(AF_INET, SOCK_STREAM, 0)))
        PERROR_EXIT("create socket failed");
    
    sockaddr_in srvaddr;
    memset(&srvaddr, 0, sizeof(srvaddr));
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_port = htons(8888);
    srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int flag = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void *)&flag, sizeof(flag)))
        PERROR_EXIT("setsockopt failed");
    
    if (bind(sock, (sockaddr*)&srvaddr, sizeof(srvaddr)))
        PERROR_EXIT("bind failed");

    if (listen(sock, 128))
        PERROR_EXIT("listen failed");

    while(1)
    {
        signal(SIGUSR1, handler);
        sockaddr_in clntaddr;
        socklen_t addrlen = sizeof(clntaddr);
        memset(&clntaddr, 0, sizeof(clntaddr));
        int sock_clnt = accept(sock, (sockaddr *)&clntaddr, &addrlen);
        if (sock_clnt < 0)
            PERROR_EXIT("accept failed");
        
        std::cout << inet_ntoa(clntaddr.sin_addr) << " connected" << std::endl;

        int pid = fork();
        if (pid < 0)
        {
            PERROR_EXIT("fork failed");
        }
        else if (pid == 0)
        {
            close(sock);

            char msg[2048];
            while (1)
            {
                int ret = readline(sock_clnt, msg, sizeof(msg));
                if (ret == 0)
                { 
                    std::cout << inet_ntoa(clntaddr.sin_addr) << " disconnected" << std::endl;
                    break;
                }
                else if (ret < 0)
                {
                    PERROR_EXIT("readn failed");
                }
                
                std::cout << "recv " << ret << " bytes data: " << msg;

                ret = write(sock_clnt, msg, ret);
                if (ret < 0)
                {
                    PERROR_EXIT("writen failed");
                }
                memset(msg, 0, sizeof(msg)); 
            }
            //sleep(10);
            //close(sock_clnt);
            exit(EXIT_SUCCESS);
            kill(getppid(), SIGUSR1);
        }
        else
        {
            close(sock_clnt);
        }
    }

    return 0;
}