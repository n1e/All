#ifndef _COMMON_NET_
#define _COMMON_NET_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#define PERROR_EXIT(MSG)\
do\
{\
    perror(MSG);\
    exit(1);\
} while (0)


struct NetPack
{
    int len;
    char buffer[2048];
};

ssize_t writen(int sock, void *buf, int len)
{
    int nCount = len;
    char *buft = (char *)buf;
    while (nCount)
    {
        int ret = write(sock, buf, nCount);
        if (ret < 0 && errno == EINTR)
            continue;
        else if (ret < 0)
            return ret;
        
        nCount -= ret;
        buft += ret;
    }
    return len;
};

ssize_t readn(int sock, void *buf, int len)
{
    int nCount = len;
    char *buft = (char *)buf;
    while (nCount)
    {
        int ret = read(sock, buf, nCount);
        if (ret < 0 && errno == EINTR)
            continue;
        else if (ret == 0)
            return len - nCount;
        else if (ret < 0)
            return ret;
        
        nCount -= ret;
        buft += ret;
    }
    return len;
};


ssize_t recv_peek(int sock, void *buf, int maxlen)
{
    while (1)
    {
        int ret = recv(sock, buf, maxlen, MSG_PEEK);
        if ( ret == -1 && errno == EINTR )
            continue;
        return ret;
    }
};

ssize_t readline(int sock, void *buf, int maxlen)
{
    char *buft = (char *)buf;
    int nCount = maxlen;
    while (nCount)
    {
        int ret = recv_peek(sock, (void *)buft, nCount);
        if (ret == 0 || ret < 0)
            return ret;
        
        int nRead = ret;
        
        for ( int i = 0 ; i < nRead ; i++ )
        {
            if (buft[i] == '\n')
            {
                memset(buft, 0, maxlen);
                ret = readn(sock, buft, i + 1);
                if (ret != i + 1)
                    PERROR_EXIT("recv not equal pre_recv");
                return ret;
            }
        }

        if ( ret > nCount)
            PERROR_EXIT("get more than need");
        
        ret = readn(sock, buft, nRead); 
        if (ret != nRead)
            PERROR_EXIT("recv not equal pre_recv");
            
        nCount -= ret;
        buft += ret;
    }
    return -1;
};


ssize_t writeline(int sock, void *buf, int maxlen)
{
    char *buft = (char *)buf;
    int nCount = maxlen;
    while (nCount)
    {
        int ret = recv_peek(sock, (void *)buft, nCount);
        if (ret == 0 || ret < 0)
            return ret;
        
        int nRead = ret;
        
        for ( int i = 0 ; i < nRead ; i++ )
        {
            if (buft[i] == '\n')
            {
                memset(buft, 0, maxlen);
                ret = readn(sock, buft, i + 1);
                if (ret != i + 1)
                    PERROR_EXIT("recv not equal pre_recv");
                return ret;
            }
        }

        if ( ret > nCount)
            PERROR_EXIT("get more than need");
        
        ret = readn(sock, buft, nRead); 
        if (ret != nRead)
            PERROR_EXIT("recv not equal pre_recv");
            
        nCount -= ret;
        buft += ret;
    }
    return -1;
};


#endif