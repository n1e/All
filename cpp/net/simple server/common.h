#ifndef _COMMON_NET_
#define _COMMON_NET_

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
        int ret = write(sock, buf, len);
        if (ret < 0 && errno = EINTR)
            continue;
        else if (ret == 0)
            return len - nCount;
        
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
        int ret = read(sock, buf, len);
        if (ret < 0 && errno = EINTR)
            continue;
        else if (ret == 0)
            return len - nCount;
        
        nCount -= ret;
        buft += ret;
    }
    return len;
};


void recv_peek(int sock, void *buf, int maxlen)
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
        int ret = recv_peek(sock, buft, nCount);
        if (ret == 0 || ret < 0)
            return ret;
        
        int nLeft = ret;
        
        for ( int i = 0 ; i < nLeft ; i++ )
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
        
        ret = readn(sock, buft, nLeft); 
        if (ret != nLeft)
            PERROR_EXIT("recv not equal pre_recv");
            
        nCount -= ret;
        buft += ret;
    }
}

#endif