#include "allocate.h"


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
	//大于128就直接调用一级适配器
	if(n > (size_t) __MAX_BYTES){
		return(allocate(n));
	}
	
	my_free_list = free_list + FREELIST_INDEX(n);
	;
	result = * my_free_list;
	if(result == 0){
		//没有找到可用的free list，准备填充free list
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
	//重新填充20个节点
	int nobjs = 20;
	char* chunk  = chunk_alloc(n,&nobjs);
	union free_list_node* volatile * my_free_list;
	union free_list_node* result;
	union free_list_node* current_obj, *next_obj;
	int i;
	//如果只获取一个区块，这个区块就分配给调用者使用，free list无新节点
	if(nobjs == 1) return chunk;
	//否则准备调整free list，纳入新节点
	my_free_list = free_list + FREELIST_INDEX(n);
	//以下在chunk空间内建立free list
		result = (union free_list_node *)chunk;//这一块返回给客户端
		//以下引导free list指向新配置的空间（取自内存池）
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
	size_t bytes_left = end_free - start_free;//内存池剩余空间
	if(bytes_left >= total_bytes){
		//完全满足需求量
		result = start_free;
		start_free += total_bytes;
		return (result);
	} else if (bytes_left >=size){//不能完全满足需求量
		*nobjs = bytes_left / size;
		total_bytes = size * (*nobjs);
		result = start_free;
		start_free += total_bytes;
		return (result);
	} else {
		//内存池剩余空间连一个区块的大小都无法提供
		size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
		//以下试图让内存池的残余零头还有利用价值
			if(bytes_left > 0){
				//寻找适当的free list
				union free_list_node* volatile * my_free_list = 
						free_list + FREELIST_INDEX(bytes_left);
				//调整free list ，将残余空间编入
				((union free_list_node*)start_free) ->free_list_link = * my_free_list;
				*my_free_list = (union free_list_node*)start_free;
			}
			//配置heap空间补充内存池
			start_free = (char*)malloc(bytes_to_get);
			if(start_free == 0){
				//heap空间不足，malloc失败
				int i;
				union free_list_node* volatile * my_free_list , *p;
				//以下搜寻足够大的尚未使用的区块
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
				end_free = 0;//实在没内存用了。。
					start_free = (char*)allocate(bytes_to_get);
			}
			heap_size += bytes_to_get;
			end_free  = start_free + bytes_to_get;
			return(chunk_alloc(size,nobjs));
	}

}