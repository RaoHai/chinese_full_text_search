#include "simple_json.h"

tjson* json_create(){
	tjson* new_json = (tjson*)malloc(sizeof(tjson));
	memset(new_json,0,sizeof(tjson));
	return new_json;
}
tjson* json_add(tjson* object,char* key,char* value){
	json_node* new_node = (json_node*)malloc(sizeof(json_node));
	memset(new_node,0,sizeof(json_node));
	new_node->key = (char*)malloc(strlen(key));
	memset(new_node->key,0,strlen(key));
	strncpy(new_node->key,key,strlen(key));
	new_node->value = (char*)malloc(strlen(value));
	memset(new_node->value,0,strlen(value));
	strncpy(new_node->value,value,strlen(value));
	if(!object->front){
		object->front = new_node;
		object->tail = new_node;
	}else{
		object->tail->next = new_node;
		object->tail = new_node;
	}
	object->len ++;
	return object;
}
char* json_print(tjson* obj){

	json_node *iter = obj->front;
	char * retval=NULL;
	char * tmpval;
	int line_size;
	do{

		line_size = strlen(iter->key) + strlen(iter->value)+5;
		if(!retval){
			retval = (char*)malloc(line_size);
			memset(retval,0,line_size);
			sprintf(retval,"'%s':'%s'",iter->key,iter->value);
		}else{
			tmpval = (char*)malloc(line_size + strlen(retval)+1);
			memset(tmpval,0,line_size + strlen(retval)+1);
			sprintf(tmpval,"%s,'%s':'%s'",retval,iter->key,iter->value);
			//free(retval);
			retval = tmpval;

		}

		//sprintf(retval,",iter->key,iter->value);
		iter = iter->next;
	}while(iter);

	tmpval = (char*)malloc(strlen(retval) + 2);
	memset(tmpval,0,strlen(retval)+2);
	sprintf(tmpval,"{%s}",retval);
    	printf("retval:%s\n",tmpval);
	return tmpval;
}

/*
int main(){
	json* new_json = json_create();
	new_json = json_add(new_json,"hello","Wordl");
	new_json = json_add(new_json,"HELLO","WORLD");
	printf("%s\n",json_print(new_json));
	printf("test\n");
	getchar();
}*/
