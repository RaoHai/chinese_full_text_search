#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
/*
	简单的key-value存储，占用空间好像有点多。。。
	使用的开链
	一百万次随机存取耗时平均3.123秒
	取的时间基本可以忽略。

**/

#ifndef KV_H_INCLUDED
#define KV_H_INCLUDED

#define  KEY_VALUE_SIZE  1024


struct __key_value_node
{
	struct __key_value_node * next;
	char* key;
	void* value;
};


unsigned int hash(char *str);

struct __key_value_node* __key_value_init();
int __key_value_insert(struct __key_value_node* key_value,const char* key,void * value);
void* __key_value_get( struct __key_value_node* key_value,const char* key);
#endif /* KV_H_INCLUDED */
