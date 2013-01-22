#include "keyvalue.h"
#include "cstl\list.h"
#ifndef TRIE_TREE_INCLUDED
#define TRIE_TREE_INCLUDED

#define true 1

typedef 
	struct __key_value_node 
Trie;//其实就是一个__key_value_node



//创建根节点，根节点是一个默认2048大小的哈希表，使用拉链法（Open Hashing）解决冲突
Trie * create_root_trie();			

//创建子节点。子节点是一个可变大小的哈希表，使用开地址法Closed Hashing解决冲突
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