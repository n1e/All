#ifndef _COMMON_
#define _COMMON_
#define PERROR_EXIT(MSG)\
do\
{\
    perror(MSG);\
    exit(1);\
} while (0)


#endif