/*
	(C)Copyright 2012 rao
	a lightly key-value store.
	support data structure: string, json

	TCP端口22222
	发送中文请用GBK编码

	语法
	[字符串]
		write 'key' 'value'				将value存在key中，若该key已经有值，覆盖。
		read  'key'`					将key中存储的值取出。若key无值，返回 “k_v_no_value_saved”
		add   'key' 'value'				将value连接到原先的value后面。

	[json]
		writejson	'key' 'value'			将value的值以json的格式存在key中，若该key已经有值，覆盖。
		jsonread	'key'					将key中存储的值以json编码的方式取出。若key无值，返回 “[]”
		jsonadd		'key' 'value'			将value添加到key中，{注1}


	{注1}
		假设以下输入
			-> writejson hello world
			->read hello
			+word
			-> jsonread hello
			+"world"
			-> jsonadd hello world2
			-> jsonread hello
			+["world","world2"]
			-> read hello
			+["world","world2"]
			这个例子主要说明，同一个key的值是想通的，read/jsonread 只是读取和写入的方式不同
							  jsonadd把存储空间里的数据强制改为json格式









*/

#include "keyvalue.h"



unsigned int hash(char *str)
{
	register unsigned int h;
	register unsigned char *p;

	for(h=0, p = (unsigned char *)str; *p ; p++)
	h = 31 * h + *p;

	return h % KEY_VALUE_SIZE;
}
char* malloc_and_copy(char* source){
	char * newstr = ALLOC(char*,strlen(source));//(char*)sub_allocate(strlen(source));
	strcpy(newstr,source);

	return newstr;
}
struct __key_value_node*  __key_value_init()
{
	for(;;)
	{
		struct __key_value_node * k_v ;
		 k_v = (struct __key_value_node*)sub_allocate(KEY_VALUE_SIZE * sizeof(struct __key_value_node));
		 memset(k_v,0,KEY_VALUE_SIZE * sizeof(struct __key_value_node));
		 k_v->size = KEY_VALUE_SIZE;

		return k_v;
	}

}
struct __key_value_node*  __key_value_insert(struct __key_value_node* key_value,const char* key,char * value)
{
    return __key_value_insert_refer(key_value,key,value,0);
}
struct __key_value_node*  __key_value_insert_refer(struct __key_value_node* key_value,const char* key,char * value,int refer)
{
	int hashed = hash((char*)key);
	int value_len = strlen(value)+1;
	int key_len = strlen((char*)key)+1;
	struct __key_value_node *newnode;
	struct __key_value_node *iter =  &key_value[hashed];
	while(iter&&iter->next!=NULL)
	{
		if(strcmp(iter->key,key)==0) {
			printf("iter->key:%s,key:%s\n",iter->key,key);
			break;
		}
		iter = iter->next;
	}
	if(!iter->key || !iter->next){
		newnode = (struct __key_value_node *)sub_allocate(sizeof(struct __key_value_node));
		memset(newnode,0,sizeof(struct __key_value_node));
		iter->next = newnode;
	}else{
		printf("rewrite\n");
	}
	if(!iter->lock){
		//A Pessimistic Concurrency Control
		iter->lock = 1;
		 iter->key =(char*)malloc(key_len); //malloc_and_copy((char*)key);
        		memset(iter->key,'\0',key_len);
        		strcpy(iter->key,key);
		if(refer == 0){
		    //key value storage will deep copy the key and value
           			 iter->value = (char*)malloc(value_len);
            			memset(iter->value,'\0',value_len);
            			strcpy(iter->value,value);
		}
		if(refer == 1){
		    //key value storage will save the reference of the value.
		    iter->value = value;
		}

		iter->lock = 0;

	}

	return iter;

}
struct __key_value_node* __key_value_get_node(struct __key_value_node* key_value,const char* key){
	int hashed = hash((char*)key);
	struct __key_value_node *iter;
	if(NULL == key_value[hashed].key ) return NULL;
	if(strcmp(key_value[hashed].key,key)==0) return &(key_value[hashed]);
	iter = key_value[hashed].next;
	while(iter&&iter->next!=NULL)
	{
		if(iter->key && strcmp(iter->key,key)==0) return iter;
		iter = iter->next;
	}

	return NULL;
}
char* __key_value_get(struct __key_value_node* key_value,const char* key)
{
	int hashed = hash((char*)key);
	struct __key_value_node *iter;
	if(key_value[hashed].key && strcmp(key_value[hashed].key,key)==0) return key_value[hashed].value;
	iter = key_value[hashed].next;
	while(iter&&iter->next!=NULL)
	{
		if(iter->key && strcmp(iter->key,key)==0) return iter->value;
		iter = iter->next;
	}

	return NULL;
}


struct __key_value_node* get_from_pool(int size)
{
	struct __key_value_node* k_v_return;
	assert(size<=k_v_pool->pool_deep-k_v_pool->water_deep);
	k_v_return = k_v_pool->pt;
	k_v_pool->pt += size;
	k_v_pool->water_deep+=size;
//	if(k_v_pool->water_deep>KEY_VALUE_SIZE) k_v_return->pos = k_v_pool->water_deep;
	//printf("get mem from pool:%d,water_deep:%d;\n",size,k_v_pool->water_deep);
	return k_v_return;
}
void __dump(FILE* fp,char* key,char* value){

	char* num;
	num = (char*)malloc(sizeof(char)*sizeof(int));
	memset(num,0,sizeof(char)*sizeof(int));
    sprintf(num,"%d",1);
	fwrite(num,sizeof(int),1,fp);
    //printf("write:1");
    	memset(num,0,sizeof(char));
	sprintf(num,"%d",strlen(key));
    //printf("%d",strlen(key));
	fwrite(num,sizeof(int),1,fp);

	fwrite(key,strlen(key),1,fp);
	//printf("%s",key);
	memset(num,0,sizeof(char));
	sprintf(num,"%d",strlen(value));
	//printf("%d",strlen(key));
	//itoa(strlen(value),num,10);
	fwrite(num,sizeof(int),1,fp);
	fwrite(value,strlen(value),1,fp);
   	 fwrite("\n",sizeof(char),1,fp);
    //printf("%s\n",value);

}


void __recover(struct __key_value_node* k_v){
	FILE *fp;
	int num;
	int i = 0;
	char* readbuf = (char*)malloc(sizeof(int));
	char* keybuf;
	char* valuebuf;
	memset(readbuf,0,sizeof(int));
	fp = fopen("dict.lib","r");
	if(!fp){
		return;
	}
	do{
		//readbuf = (char*)realloc(readbuf,sizeof(int));
		//readbuf = (char*)malloc(sizeof(int));//(char*)sub_allocate(sizeof(int));
		memset(readbuf,0,sizeof(int));
		fread(readbuf,sizeof(int),1,fp);
		printf("read:next->%s\n",readbuf);
		if(!strcmp(readbuf,"1")==0){
		    printf("file format error!\n");
		    return;
		    }
		fread(readbuf,sizeof(int),1,fp);
		num = atoi(readbuf);
		//printf("read:num->%d\n",num);
		if(num>0){
			keybuf = (char*)malloc(num);
			memset(readbuf,0,num);
		}

		fread(keybuf,num*sizeof(char),1,fp);
		keybuf[num]='\0';
		//printf("read:key->%s,strlen:%d\n",keybuf,strlen(keybuf));
		fread(readbuf,sizeof(int),1,fp);
		num = atoi(readbuf);
		//printf("read:num->%d\n",num);
		if(num>0){
			valuebuf = (char*)malloc(num+1);
			memset(valuebuf,0,num+1);
		}
		fread(valuebuf,num,1,fp);
		valuebuf[num]='\0';
		printf("read:key->%s,value:%s\n",keybuf,valuebuf);
		__key_value_insert(k_v,keybuf,valuebuf);

		i++;
	}
	while(1);
	fclose(fp);
	printf("recover count:%d\n",i);
}
