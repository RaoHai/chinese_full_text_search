#include "allocate.h"
#ifndef ALLOCATE_C_INCLUDED
#define ALLOCATE_C_INCLUDED

static void * allocate(size_t n){
	void* result = malloc(n);
	if(result == 0) result = oom_malloc(n);
	return result;
}

static void deallocate(void *p,size_t n){
	free(p);
}

static void * reallocate(void *p, size_t old_sz, size_t new_sz){
	void * result = realloc(p,new_sz);
	if(result == 0) result = oom_realloc(p,new_sz);
	return result;
}

static void * oom_malloc(size_t n){
	void * result;
	for(;;){
		result = malloc(n);
		if(result)return result;
	}
}

static void * oom_realloc(void* p,size_t n){
	void * result;
	for(;;){
		result = realloc(p,n);
		if(result) return result;
	}
}


//Sub allocate

static void* sub_allocate(size_t n){
	
	union free_list_node * volatile * my_free_list;
	union free_list_node * result;
	//����128��ֱ�ӵ���һ��������
	if(n > (size_t) __MAX_BYTES){
		return(allocate(n));
	}
	
	my_free_list = free_list + FREELIST_INDEX(n);
	
	result = * my_free_list;
	if(result == 0){
		//û���ҵ����õ�free list��׼�����free list
		void* r = refill(ROUND_UP(n));
		return r;
	}

	*my_free_list  = result -> free_list_link;
	return (result);

}
static void sub_deallocate(void *p,size_t n){
	union free_list_node *q = (union free_list_node*)0;
	union free_list_node * volatile * my_free_list;
	if ( n> (size_t) __MAX_BYTES){
		deallocate(p,n);
		return;
	}
	my_free_list = free_list + FREELIST_INDEX(n);
	q ->free_list_link = * my_free_list;
	*my_free_list = q;
}
void * refill(size_t n){
	//�������20���ڵ�
	int nobjs = 20;
	char* chunk  = chunk_alloc(n,&nobjs);
	union free_list_node* volatile * my_free_list;
	union free_list_node* result;
	union free_list_node* current_obj, *next_obj;
	int i;
	//���ֻ��ȡһ�����飬�������ͷ����������ʹ�ã�free list���½ڵ�
	if(nobjs == 1) return chunk;
	//����׼������free list�������½ڵ�
	my_free_list = free_list + FREELIST_INDEX(n);
	//������chunk�ռ��ڽ���free list
		result = (union free_list_node *)chunk;//��һ�鷵�ظ��ͻ���
		//��������free listָ�������õĿռ䣨ȡ���ڴ�أ�
		* my_free_list = next_obj = (union free_list_node*)(chunk + n);
		for(i=1;;i++){
			current_obj = next_obj;
			next_obj =(union free_list_node *)((char *)next_obj + n);
			if(nobjs - 1 == i){
				current_obj ->free_list_link = 0;
				break;
			}else{
				current_obj ->free_list_link = next_obj;
			}
		}
		return (result);

}

char* chunk_alloc(size_t size,int * nobjs){
	char * result;
	size_t total_bytes = size * (*nobjs);
	size_t bytes_left = end_free - start_free;//�ڴ��ʣ��ռ�
	if(bytes_left >= total_bytes){
		//��ȫ����������
		result = start_free;
		start_free += total_bytes;
		return (result);
	} else if (bytes_left >=size){//������ȫ����������
		*nobjs = bytes_left / size;
		total_bytes = size * (*nobjs);
		result = start_free;
		start_free += total_bytes;
		return (result);
	} else {
		//�ڴ��ʣ��ռ���һ������Ĵ�С���޷��ṩ
		size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
		//������ͼ���ڴ�صĲ�����ͷ�������ü�ֵ
			if(bytes_left > 0){
				//Ѱ���ʵ���free list
				union free_list_node* volatile * my_free_list = 
						free_list + FREELIST_INDEX(bytes_left);
				//����free list ��������ռ����
				((union free_list_node*)start_free) ->free_list_link = * my_free_list;
				*my_free_list = (union free_list_node*)start_free;
			}
			//����heap�ռ䲹���ڴ��
			start_free = (char*)malloc(bytes_to_get);
			if(start_free == 0){
				//heap�ռ䲻�㣬mallocʧ��
				int i;
				union free_list_node* volatile * my_free_list , *p;
				//������Ѱ�㹻�����δʹ�õ�����
				for(i=size; i< __MAX_BYTES ; i+= __ALIGN){
					my_free_list = free_list + FREELIST_INDEX(i);
					p = *my_free_list;
					if(p != 0){
						*my_free_list = p ->free_list_link;
						start_free = (char*) p;
						end_free = start_free + i;
						return(chunk_alloc(size,nobjs));
					}
				}
				end_free = 0;//ʵ��û�ڴ����ˡ���
					start_free = (char*)allocate(bytes_to_get);
			}
			heap_size += bytes_to_get;
			end_free  = start_free + bytes_to_get;
			return(chunk_alloc(size,nobjs));
	}

}

#endif