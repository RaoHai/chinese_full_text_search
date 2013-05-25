#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
//#include <process.h>
//#include <windows.h>


#include "cstlib.h"
#include "event.h"
//#include "keyvalue.h"

#include "protocol.h"

#define SIZE_ARR 1024

#define LOG_TO_FILE



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
char* tcp_recv_callback(char* msg){
	//printf("tcp_recv_callback:%s\n",msg );

	return command_parser(msg);
	//return command(msg);
}
int main(){


	/*
	key_value=__key_value_init();

	*/
//	Trie  temp;
//	char * retval;
//	char * cnw = "软件工程";

   // StrMap *backup;
    //backup =StrMap_new();
    	register_command();

    	set_user_recv_callback(tcp_recv_callback);
       	root = create_root_trie();

        //p_command_parser("*3\r\n$write\r\n$中国\r\n$world\r\n");

        //printf(">>>%s\n",command_parser("get hello"));
	//insert_backup = __key_value_init();

	//Trie * cur ;
	//Trie * child;

	/*
	root_insert(root,"软","0");
	cur = get_root_trie_by_key(root,"软");

	child = create_child_trie("件","1");

	child = trie_add_child(&child,"体","1");

	trie_insert(&cur,child);

	cur = get_child_trie_by_key(cur->child,"体");
	child  =  create_chil d_trie("件","工");
	trie_insert(&cur,child);*/
	/*
	trie_save(root,"软件","0.03");
	trie_save(root,"软件工程","0.025");
	trie_save(root,"软件开发","no data");
	//*/
	//trie_get(root,"软件工程");
	//trie_save(root,"软件设计");
	//trie_print(cur);
	trie_recover();


	//dbg_heap_dump("");
	//command_parser("read hello");
	init_server();


	/*
	retval = trie_search(root,"爱");

	printf("search \"软\":%s\n",retval);
	*/
	getchar();
    	return 0;

}

void key_value_test()
{
	struct __key_value_node* key_value=__key_value_init();
	char* result;
//	int error=0;
//	int i;
//	clock_t start; clock_t end;
//	char* s1,* s2;
//	char *s;

	__key_value_insert(key_value,"Hello","World");
	result = __key_value_get(key_value,"Hello");
	printf("get key:%s,value:%s...\n","Hello",result);
}
