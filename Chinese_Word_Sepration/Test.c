#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <process.h>
//#include <windows.h>


#include "cstlib.h"
#include "key_value_tcp.h"
#include "keyvalue.h"
#include "trie_tree.h"

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
	
	/*
	key_value=__key_value_init();
	tcp_start_up();
	*/
	Trie  temp;
	char * retval;
	char * cnw = "软件工程";
	Trie * root = create_root_trie();
	Trie * cur ;
	Trie * child;

	/*
	root_insert(root,"软","0");
	cur = get_root_trie_by_key(root,"软");

	child = create_child_trie("件","1");

	child = trie_add_child(&child,"体","1");

	trie_insert(&cur,child);
	
	cur = get_child_trie_by_key(cur->child,"体");
	child  =  create_chil d_trie("件","工");
	trie_insert(&cur,child);*/
	trie_save(root,"软件","0.03");
	trie_save(root,"软件工程","0.025");
	trie_save(root,"软件开发","no data");

	trie_get(root,"软件工程");
	//trie_save(root,"软件设计");
	//trie_print(cur);
	
	
	
	/*
	retval = trie_search(root,"爱");

	printf("search \"软\":%s\n",retval);
	*/
	getchar();

	
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