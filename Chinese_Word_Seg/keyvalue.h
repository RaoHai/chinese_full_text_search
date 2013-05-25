#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <assert.h>
#include <string.h>
//#include <process.h>


#include "allocate.c"
#include "cstlib.h"
#include "cJSON.h"

//#include "file_service_thread.h"
/*
	简单的key-value存储，占用空间好像有点多。。。
	使用的开链
	一百万次随机存取耗时平均3.123秒
	取的时间基本可以忽略。

**/

#ifndef KV_H_INCLUDED
#define KV_H_INCLUDED

#define inline __inline

#define  KEY_VALUE_SIZE  2048				//哈希表的表头尺寸
#define  MAX_KEY_VALUE_SIZE 2048
#define  NO_VALUE "k_v_no_value_saved"
#define  KEY_VALUE_MUTIPLUS 10
#define  MAX_NUM_OF_POOL 100				//内存池数组的最大值


/*
 *	此处的宏定义的作用如下
 *  假如原先的调用是
		newnode = (struct __key_value_node *)sub_allocate(sizeof(struct __key_value_node));
		memset(newnode,0,sizeof(struct __key_value_node));
 *	那么现在可以简写成
 *		newnode = ALLOC(struct __key_value_node*,sizeof(struct __key_value_node));
 *	这么着好像可以偷懒一下
 *  或者之后可以用typeof来坐坐，那type参数就可以不用传了
 */

#define		ALLOC(type,size)	((type)m_ALLOC(size))
//假装inline一下……
static inline void* m_ALLOC(size_t n){
	void* ret =  sub_allocate(n);
	memset(ret,0,n);
	return ret;
}


char* malloc_and_copy(char* source);




struct __key_value_node
{
	struct __key_value_node * next;
	struct __key_value_node * child;
	int		lock;
	int		size;
	char*	key;
	char*	value;
};

struct __key_value_node* key_value;
struct _k_v_pool{
	struct __key_value_node* pt;//当前的指针
	struct __key_value_node* k_v;//存储hash表
	int pool_deep; //池深
	int water_deep; //水深

} *k_v_pool;



unsigned int hash( char *str);

struct __key_value_node* __key_value_init();
struct __key_value_node* __key_value_insert( struct __key_value_node* key_value,const char* key,char * value);
struct __key_value_node* __key_value_insert_refer( struct __key_value_node* key_value,const char* key,char * value,int refer);
char* __key_value_get( struct __key_value_node* key_value,const char* key);
struct __key_value_node* __key_value_get_node(struct __key_value_node* key_value,const char* key);



void __recover(struct __key_value_node*);
void _dump(FILE*,char*,char*);

/*k_v 池*/
//int _k_v_pool_init();
//struct __key_value_node* get_from_pool( int);
//void _mem_pool_trace();

#endif /* KV_H_INCLUDED */
