#ifndef PROTOCOL_H_INCLUDED
#define PROTOCOL_H_INCLUDED
#include "trie_tree.h"

struct __key_value_node* command_dict;
typedef  struct command_map{
	char* command;
	int argc;
	void* callback_func;
	//void* callback_args;
} cmd_map;

typedef char*(*func)(Trie* trie,void*);
void register_command();
void* p_command_parser(char* command);
int get_next_int(char**);
char* get_next_str(char**);
void func_test(void);

void* write_command(Trie*,void*);
void* read_command(Trie*,void*);

#endif // PROTOCOL_H_INCLUDED
