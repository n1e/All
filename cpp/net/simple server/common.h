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

#endif