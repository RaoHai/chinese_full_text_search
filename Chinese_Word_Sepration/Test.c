#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <process.h>
//#include <windows.h>


#include "cstlib.h"
#include "key_value_tcp.h"
#include "keyvalue.h"


#define SIZE_ARR 1024




void tcp_recv_callback(char* msg){
	printf("msg recv:%s\n",msg);
}
char * rand_str(int in_len) 
{ 
  char *__r = (char *)malloc(in_len + 1); 
   int i; 
  if (__r == 0) 
  { 
    return 0; 
  } 
 srand(time(NULL) + rand());    //初始化随机数的种子
  for (i = 0; i  < in_len; i++) 
  { 
    __r[i] = rand()%94+32;      //控制得到的随机数为可显示字符
  } 
  __r[i] = 0; 
  return __r; 
} 
void key_value_test();



int main(){
	


	unsigned pid;
	int i;
	//char * callback_value = (char*) malloc ( sizeof(char*));
	//char * callback_value = (char*)sub_allocate(sizeof(char*));
	char * callback_value = ALLOC(char*,sizeof(char*));
	int count=0;
	//char * command = (char*) malloc (SIZE_ARR * sizeof(char*));
	char * command = (char*)sub_allocate(SIZE_ARR * sizeof(char*));
	


	
	char* s1,* s2;
	int error=0;
	char *out;
	cJSON *root;
	root=cJSON_CreateString("hello");
	out=cJSON_Print(root);
	printf("%s\n",out);

	key_value=__key_value_init();
	tcp_start_up();
	
	//__key_value_insert(key_value,"Hello","World");
	//result = __key_value_get(key_value,"Hello");
	//printf("get key:%s,value:%s\n","Hello",result);
	
	
	//if(init_file_service())
	//	printf("init successed;\n");
	
	/*
	for(i=0;i<10000;i++)
	{
		
		//s1 = rand_str(5);
		//s2 = rand_str(5);
		s1 = (char*)sub_allocate(10);
		s2 = (char*)sub_allocate(10);
		itoa(i,s1,10);
		itoa(i,s2,10);
		//printf("creating key:%s,value:%s...\n",s1,s2);
		__key_value_insert(key_value,s1,s2);
		 result = __key_value_get(key_value,s1);
		//printf("get key:%s,value:%s...\n",s1,result);
		if(strcmp(result,s2)!=0) error++;
	}

	
	printf("error count:%d\n",error);
	// */
	


	//struct lock_free_queue *file_write_queue = new_lock_free_queue();
	/* = (struct lock_free_queue *)malloc(sizeof(struct lock_free_queue *));
	memset(file_write_queue,0,sizeof(struct lock_free_queue *));
	file_write_queue->front = file_write_queue->tail=NULL;
	*/
	//_beginthreadex(NULL,0, (unsigned (__stdcall *) (void *))thread_proc,(void *)file_write_queue,0,&pid);
	
	//trace_queue();
	/*
	struct __key_value_node* key_value=__key_value_init();
	char* result;
	int error=0;
	int i;
	clock_t start; clock_t end;
	char* s1,* s2;
	char *s;

	


	__key_value_insert(key_value,"Hello","World");
	result = __key_value_get(key_value,"Hello");
	printf("get key:%s,value:%s...\n","Hello",result);
	*/
	
	
	
	/*
	start = clock();

	for(i=0;i<100;i++)
	{
		
		s1 = rand_str(5);
		s2 = rand_str(5);
		
		//printf("creating key:%s,value:%s...\n",s1,s2);
		__key_value_insert(key_value,s1,s2);
		 result = (char*)__key_value_get(key_value,s1);
		//printf("get key:%s,value:%s...\n",s1,result);
		if(result!=s2) error++;
	}
	end = clock();
	printf("1,000,000times save and reaload costs:%f\n", (end - start)/(float)CLK_TCK);
	printf("error count:%d\n",error);
	*/
	/*
	_mem_pool_trace();

	printf("init tcp server...\n");
	_tcp_int_and_listen(80,tcp_recv_callback);
	*/
	
	/*
 unsigned pid;
 int count=0;
	char * command = (char*) malloc (SIZE_ARR * sizeof(char*));
 while(count<5)
 {
	  _beginthreadex(NULL,0, (unsigned (__stdcall *) (void *))thread_proc,(void *)count,0,&pid);
	  Sleep( 10);
	  printf("%d\n",pid);
	  count++;
 }
 */


	
	

}

void key_value_test()
{
	struct __key_value_node* key_value=__key_value_init();
	char* result;
	int error=0;
	int i;
	clock_t start; clock_t end;
	char* s1,* s2;
	char *s;

	__key_value_insert(key_value,"Hello","World");
	result = __key_value_get(key_value,"Hello");
	printf("get key:%s,value:%s...\n","Hello",result);
}