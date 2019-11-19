#include "../simple server/common.h"
int main()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        PERROR_EXIT("create socket failed");

    sockaddr_in srv_addr;
    memset(&srv_addr, 0, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(8888);
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int nOpt = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void *)&nOpt, sizeof(nOpt)))
        PERROR_EXIT("setsockopt reuseraddr failed");

    if (bind(sock, (sockaddr*)&srv_addr, sizeof(srv_addr)))
        PERROR_EXIT("bind failed");

    if (listen(sock, 256))
        PERROR_EXIT("listen failed");

    fd_set fd_read;
    int maxfd = sock;
    int second_maxfd = sock;
    int fd_arr[2048] = {0};

    while (1)
    {
        FD_ZERO(&fd_read);
        FD_SET(sock, &fd_read);

        for (int i = 0; i < maxfd + 1; i++)
        {
            if (fd_arr[i] == 1)
            {
                FD_SET(i, &fd_read);
            }
        }

        int nRet = select(maxfd + 1, &fd_read, NULL, NULL, NULL);
        if (nRet == -1)
        {
            if (errno==EAGAIN||errno==EWOULDBLOCK||errno==EINTR)
                continue;
            else
                PERROR_EXIT("select failed");           
        }
        else if (nRet == 0)
            continue;
        else
        {
            if (FD_ISSET(sock, &fd_read))
            {
                sockaddr_in addr_clnt;
                socklen_t len = sizeof(addr_clnt);
                int sock_clnt = accept(sock, (sockaddr*)&addr_clnt, &len);
                if (sock_clnt < 0)
                {
                    PERROR_EXIT("accept failed");
                }
                else
                {
                    std::cout << inet_ntoa(addr_clnt.sin_addr) <<" connected" << std::endl;
                    fd_arr[sock_clnt] = 1;
                    if (sock_clnt > maxfd)
                    {
                        second_maxfd = maxfd;
                        maxfd = sock_clnt;
                    }
                    nRet--;
                }
            }

            int i = 0;
            while (nRet > 0 && i < maxfd + 1)
            {
                if (FD_ISSET(i, &fd_read))
                {
                    char msg[2048] = {0};
                    int nLen = readline(i, msg, sizeof(msg));
                    if (nLen < 0)
                    {
                        PERROR_EXIT("readline failed");
                    }
                    else if (nLen == 0)
                    {
                        sockaddr_in clntaddr;
                        socklen_t len = sizeof(clntaddr);
                        getpeername(i, (sockaddr*)&clntaddr, &len);
                        std::cout << "client [" << inet_ntoa(clntaddr.sin_addr) << "] disconnected" << std::endl;
                        fd_arr[i] = 0;
                        if (maxfd == i && second_maxfd > 0)
                            maxfd = second_maxfd;
                    }
                    else
                    {
                        std::cout << "recv :" << msg;
                        int nlen = writen(i, msg, strlen(msg));
                        if (nlen < 0)
                            PERROR_EXIT("writen failed");
                    }
                    
                    nRet--;
                }
                i++;
            }
        }
        
    }


    return 0;
}