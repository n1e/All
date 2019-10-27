#include "../simple server/common.h"
void handler(int sig)
{
    std::cout << "recv a sig: " << sig << std::endl;
    exit(EXIT_SUCCESS);
}
int main()
{
    signal(SIGPIPE, handler);
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        PERROR_EXIT("create socket failed");
    
    sockaddr_in srv_addr;
    memset(&srv_addr, 0, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(8888);
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int flag = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)))
        PERROR_EXIT("setsockopt failed");

    if (bind(sock, (sockaddr*)&srv_addr, sizeof(srv_addr)))
        PERROR_EXIT("bind failed");

    if (listen(sock, 256))
        PERROR_EXIT("listen failed");

    sockaddr_in clnt_addr;
    socklen_t len = sizeof(clnt_addr);
    int clnt_sock = accept(sock, (sockaddr*)&clnt_addr, &len);
    if (clnt_sock < 0)
        PERROR_EXIT("accept failed");

    std::cout << "client connected : " << inet_ntoa(clnt_addr.sin_addr) << std::endl;

    sleep(20);
    std::cout << "slepp 10 s" << std::endl;

    // char buffer[1024] = {0};
    // int ret = read(clnt_sock, buffer, sizeof(buffer));
    // std::cout << "ret is " << ret << std::endl;
    // if ( ret == 0 )
    //     std::cout << "client closed" << std::endl;
    // else if (ret < 0)
    //     PERROR_EXIT("read failed");

    char msg[1024] = "you can't recv";
    int send_len = write(clnt_sock, msg, 1);
    std::cout << "1 len :" << send_len << std::endl;
    send_len = write(clnt_sock, msg + 1, strlen(msg) - 1);
    std::cout << "2 len :" << send_len << std::endl;

    if (0 == read(clnt_sock, msg, sizeof(msg)))
        PERROR_EXIT("client closed");

}