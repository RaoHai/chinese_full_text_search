#include "keyvalue.h"
#include "cstl\list.h"
#ifndef TRIE_TREE_INCLUDED
#define TRIE_TREE_INCLUDED

#define true 1

typedef 
	struct __key_value_node 
Trie;//��ʵ����һ��__key_value_node



//�������ڵ㣬���ڵ���һ��Ĭ��2048��С�Ĺ�ϣ��ʹ����������Open Hashing�������ͻ
Trie * create_root_trie();			

//�����ӽڵ㡣�ӽڵ���һ���ɱ��С�Ĺ�ϣ��ʹ�ÿ���ַ��Closed Hashing�����ͻ
Trie * create_child_trie(char* key,char* value);


Trie ** trie_add_child(Trie**,char* key,char* value);
//Trie * trie_add_child(Trie*,Trie*);

Trie * get_root_trie_by_key(Trie*,char* key);
Trie * get_child_trie_by_key(Trie*,char* key);

Trie*   trie_insert(Trie*,Trie*);

Trie*  root_insert(Trie*,char*,char*);
char*  trie_search(Trie*,char*);
char*  trie_get(Trie*,char*);

void  trie_remove_child(Trie*,Trie*);

void  trie_cpy(Trie*,Trie*);
void  trie_print(Trie*);
void  trie_print_ln(Trie*);

//int	trie_save(Trie*,char *);
int	trie_save(Trie*,char *,char*);
int trie_save_letter(Trie*,char*);

unsigned int arg_hash(char *str,int size);
char* get_next_word(char*,int*);
int empty(Trie*);


#endif