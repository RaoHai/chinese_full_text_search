#include "keyvalue.h"


unsigned int hash(char *str)
{
register unsigned int h;
register unsigned char *p; 

for(h=0, p = (unsigned char *)str; *p ; p++)
h = 31 * h + *p; 

return h % KEY_VALUE_SIZE;
}

struct __key_value_node*  __key_value_init()
{
	for(;;)
	{
		struct __key_value_node * k_v =   (struct __key_value_node*) malloc (KEY_VALUE_SIZE*sizeof(struct __key_value_node));
		if (k_v) memset(k_v,0,KEY_VALUE_SIZE*sizeof(struct __key_value_node));
		return k_v;
	}

}
int __key_value_insert(struct __key_value_node* key_value,const char* key,void * value)
{
	int hashed = hash((char*)key);
	struct __key_value_node *newnode;

	struct __key_value_node *iter =  &key_value[hashed];
	while(iter&&iter->next!=NULL)
	{
		if(iter->key == key) break;
		iter = iter->next;
	}
	iter->value = value;
	iter->key = (char*)key;
	
	newnode = (struct __key_value_node*) malloc(sizeof(struct __key_value_node));
	if (newnode) memset(newnode,0,sizeof(struct __key_value_node));
	iter->next = newnode;
	
	return 0;

}
void* __key_value_get(struct __key_value_node* key_value,const char* key)
{
	int hashed = hash((char*)key);
	struct __key_value_node *iter;
	if(key_value[hashed].key == key) return key_value[hashed].value;
	iter = key_value[hashed].next;
	while(iter&&iter->next!=NULL)
	{
		if(iter->key==key) return iter->value;
		iter = iter->next;
	}
	return "OK";
}
int main(){
	char* result;
	struct __key_value_node* key_value=__key_value_init();
	//printf("creating key:%s,value:%s...\n",s1,s2);
	__key_value_insert(key_value,"Hello","World");
	result = (char*)__key_value_get(key_value,"Hello");
	printf("get key:%s,value:%s...\n","Hello",result);

	getchar();
}

/*
	×Ô¶¯»¯²âÊÔ
	__key_value_node* key_value=__key_value_init();
	int error=0;
	clock_t start; clock_t end;
	start = clock();

	for(int i=0;i<1000000;i++)
	{
		char s1[5],s2[5];
		rand_str(s1,5);
		rand_str(s2,5);
		
		//printf("creating key:%s,value:%s...\n",s1,s2);
		__key_value_insert(key_value,s1,s2);
		char* result = (char*)__key_value_get(key_value,s1);
		//printf("get key:%s,value:%s...\n",s1,result);
		if(result!=s2) error++;
	}
	 end = clock();
	printf("1,000,000times save and reaload costs:%f\n", (end - start)/(float)CLK_TCK);
	printf("error count:%d\n",error);

*/
