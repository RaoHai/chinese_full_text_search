/*
 * 一些需要用到的库函数
 */
#ifndef CSTLIB_H_INCLUDED
#define CSTLIB_H_INCLUDED

#define LOWER(c)            (unsigned char)(c | 0x20)
#define IS_ALPHA(c) ((('a' <= (c)) && ((c) <= 'z')) || (('A' <= (c)) && ((c) <= 'Z')))

char* L(char* s);



#endif