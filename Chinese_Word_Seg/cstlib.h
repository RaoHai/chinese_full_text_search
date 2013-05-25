
/*
 * 一些需要用到的库函数
 */
#ifndef CSTLIB_H_INCLUDED
#define CSTLIB_H_INCLUDED

	#include <stdlib.h>
	#include <stdio.h>
	#include <memory.h>
	#include <ctype.h>
	#include <stdarg.h>
	//#include "memleak\memleak.h"
//#define LOG_TO_FILE

#define LOWER(c)            (unsigned char)(c | 0x20)
#define IS_ALPHA(c) ((('a' <= (c)) && ((c) <= 'z')) || (('A' <= (c)) && ((c) <= 'Z')))

char* L(char* s);
char * trim(char * ptr);
char** cSplit(char *string, char spliter);
void freeStringsMemory(char **strings);
void k_log(const char*,...);

FILE* tp;

#endif
