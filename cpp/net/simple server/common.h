#ifndef _COMMON_NET_
#define _COMMON_NET_

#define PERROR_EXIT(MSG)\s
do\
{\
    perror(MSG);\
    exit(1);\
} while (0)


#endif