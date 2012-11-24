#include <stdlib.h>
#include <memory.h>
#include <stdio.h>

#ifndef ALLOCATE_H_INCLUDED
#define ALLOCATE_H_INCLUDED

//一级适配器
static void* allocate(size_t n);
static void  deallocate(void* p,size_t);
static void* reallocate(void* p,size_t,size_t);
static void* oom_malloc(size_t n);
static void* oom_realloc(void *,size_t);

//二级适配器
enum {__ALIGN = 8};				//小型区块的上调边界
enum {__MAX_BYTES = 128};		//小型区块的上限
enum {_NFREELISTS = __MAX_BYTES / __ALIGN };//free-lists 个数

static size_t ROUND_UP(size_t bytes){	//将bytes上调至 8 的倍数
	return (((bytes) + __ALIGN-1) & ~(__ALIGN -1));
}

union	free_list_node{
	union free_list_node * free_list_link;
	char client_data[1];
};

static union free_list_node* volatile free_list[_NFREELISTS]  
			= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static size_t FREELIST_INDEX(size_t bytes){
	return (((bytes) + __ALIGN-1) / __ALIGN -1);
}

static void* refill(size_t n);//返回一个大小为n的对象，并可能加入大小为n的其他区块到free list
//配置一大块空间，可容纳nobjs个大小为size的模块
//如果配置nobjs个区块有所不便，nobjs可能会降低
static char* chunk_alloc(size_t size,int * nobjs/* & nobjs */);

//Chunk allocation state
static char* start_free = 0;
static char* end_free = 0;
static size_t heap_size = 0;

static void* sub_allocate(size_t n);
static void sub_deallocate(void *p,size_t n);
static void sub_reallocate(void* p,size_t old_size,size_t new_sz);




#endif
