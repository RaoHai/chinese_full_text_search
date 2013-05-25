#include <stdlib.h>
#include <memory.h>
#include <stdio.h>

#ifndef ALLOCATE_H_INCLUDED
#define ALLOCATE_H_INCLUDED

//һ��������
static void* allocate(size_t n);
static void  deallocate(void* p,size_t);
static void* reallocate(void* p,size_t,size_t);
static void* oom_malloc(size_t n);
static void* oom_realloc(void *,size_t);

//����������
enum {__ALIGN = 8};				//С��������ϵ��߽�
enum {__MAX_BYTES = 128};		//С�����������
enum {_NFREELISTS = __MAX_BYTES / __ALIGN };//free-lists ����

static size_t ROUND_UP(size_t bytes){	//��bytes�ϵ��� 8 �ı���
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

static void* refill(size_t n);//����һ����СΪn�Ķ��󣬲����ܼ����СΪn���������鵽free list
//����һ���ռ䣬������nobjs����СΪsize��ģ��
//�������nobjs�������������㣬nobjs���ܻή��
static char* chunk_alloc(size_t size,int * nobjs/* & nobjs */);

//Chunk allocation state
static char* start_free = 0;
static char* end_free = 0;
static size_t heap_size = 0;

static void* sub_allocate(size_t n);
static void sub_deallocate(void *p,size_t n);
static void sub_reallocate(void* p,size_t old_size,size_t new_sz);




#endif
