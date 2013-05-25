#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

typedef struct json_node{
	char* key;
	char* value;
	struct json_node * next;

}json_node;
typedef struct json{
	json_node * front;
	json_node * tail;
	int len;
}tjson;

tjson* json_create();
tjson* json_add(tjson*,char*,char*);
char* json_print(tjson*);
