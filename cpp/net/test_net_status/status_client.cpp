#include "../simple server/common.h"

int main()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        PERROR_EXIT("create sock failed");

    sockaddr_in srv_addr;
    memset(&srv_addr, 0, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(8888);
    srv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (sockaddr*)&srv_addr, sizeof(srv_addr)))
        PERROR_EXIT("connect failed");
    
    sleep(10);
    char msg[1024] = "you can't recv";
    int send_len = write(sock, msg, 1);
    std::cout << "1 len :" << send_len << std::endl;
    send_len = write(sock, msg + 1, strlen(msg) - 1);
    std::cout << "2 len :" << send_len << std::endl;

    sleep(30);
    close(sock);
}