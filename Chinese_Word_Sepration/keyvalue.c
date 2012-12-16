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
	//char* newstr = (char*)malloc(strlen(source)*sizeof(char));
	char * newstr = ALLOC(char*,strlen(source));//(char*)sub_allocate(strlen(source));
//	memset(newstr,0,strlen(source)*sizeof(char));
	strcpy(newstr,source);
	return newstr;
}
struct __key_value_node*  __key_value_init()
{
	for(;;)
	{
		struct __key_value_node * k_v ;
		// k_v = (struct __key_value_node*) malloc (KEY_VALUE_SIZE * sizeof(struct __key_value_node));
		 k_v = (struct __key_value_node*)sub_allocate(KEY_VALUE_SIZE * sizeof(struct __key_value_node));
		 memset(k_v,0,KEY_VALUE_SIZE * sizeof(struct __key_value_node));
		//k_v = get_from_pool(KEY_VALUE_SIZE);
		//struct __key_value_node * k_v =   (struct __key_value_node*) malloc (KEY_VALUE_SIZE*sizeof(struct __key_value_node));
		//struct __key_value_node * k_v = (struct __key_value_node*) malloc (sizeof(struct __key_value_node));
		//if (k_v) memset(k_v,0,KEY_VALUE_SIZE*sizeof(struct __key_value_node));

		__recover(k_v);
		return k_v;
	}

}
int __key_value_insert(struct __key_value_node* key_value,const char* key,char * value)
{
	//TOFIX: 在修改数据时仍会插入新节点
	//TOFIX: 字符串太长的话strcmp会出问题
	int hashed = hash((char*)key);
	int filepos = hashed;
	int nextpos = 0 ;
	int value_len = strlen(value);
	int key_len = strlen((char*)key);
	//FILE *fp;
	//long filepos = (long)hashed;
	struct __key_value_node *newnode;
	struct __key_value_node *iter =  &key_value[hashed];
	//fp= fopen("dict.lib","rb+");
	//fseek(fp,filepos,SEEK_SET);
	while(iter&&iter->next!=NULL)
	{
		if(strcmp(iter->key,key)==0) {
			printf("iter->key:%s,key:%s\n",iter->key,key);
			break;
		}
		iter = iter->next;
	}
	if(!iter->key || !iter->next){
		//newnode = get_from_pool(1);
		
		newnode = (struct __key_value_node *)sub_allocate(sizeof(struct __key_value_node));
		memset(newnode,0,sizeof(struct __key_value_node));
		iter->next = newnode;
		//nextpos = newnode->pos;
	}else{
		printf("rewrite\n");
		//nextpos = iter->next->pos;
	}
	if(iter->pos && iter->pos > KEY_VALUE_SIZE) filepos = iter->pos;
	
	iter->value = (char*)malloc(value_len);
	memset(iter->value,0,value_len);
	//malloc_and_copy(value);
	strcpy(iter->value,value);
	iter->key =(char*)malloc(key_len); //malloc_and_copy((char*)key);
	memset(iter->key,0,key_len);
	strcpy(iter->key,key);
	//if(filepos>KEY_VALUE_SIZE) printf("\n %s -> %s \n",key,value);

	//write(filepos,iter->key,value,nextpos);//*/

	return 0;

}
char* __key_value_get(struct __key_value_node* key_value,const char* key)
{
	int hashed = hash((char*)key);
	struct __key_value_node *iter;
	//char * callback_value = (char*) malloc ( sizeof(char)*10);
	long filepos = (long)hashed;
	struct __key_value_node *_temp_k_v = (struct __key_value_node *)malloc(sizeof(struct __key_value_node));
	//printf("search key:%s(hash:%d),-->%s\n",key,hashed,key_value[hashed].key);
	if(key_value[hashed].key && strcmp(key_value[hashed].key,key)==0) return key_value[hashed].value;
	iter = key_value[hashed].next;
	while(iter&&iter->next!=NULL)
	{
		if(iter->key && strcmp(iter->key,key)==0) return iter->value;
		iter = iter->next;
	}
	//memset(callback_value,0,sizeof(char)*10);
	//read(hashed,callback_value);
	//return callback_value;
	return NO_VALUE;
}


struct __key_value_node* get_from_pool(int size)
{
	struct __key_value_node* k_v_return;
	assert(size<=k_v_pool->pool_deep-k_v_pool->water_deep);
	k_v_return = k_v_pool->pt;
	k_v_pool->pt += size;
	k_v_pool->water_deep+=size;
	if(k_v_pool->water_deep>KEY_VALUE_SIZE) k_v_return->pos = k_v_pool->water_deep;
	//printf("get mem from pool:%d,water_deep:%d;\n",size,k_v_pool->water_deep);
	return k_v_return;
}
void __dump(FILE* fp,char* key,char* value){
	char* num;
	num = (char*)malloc(sizeof(char)*sizeof(int));
			//itoa(1,num,10);
	fwrite("1",sizeof(int),1,fp);
			
	itoa(strlen(key),num,10);
	fwrite(num,sizeof(int),1,fp);
			
	fwrite(key,strlen(key),1,fp);
	//fwrite('\0',sizeof(char),1,fp);
	itoa(strlen(value),num,10);
	fwrite(num,sizeof(int),1,fp);

	fwrite(value,strlen(value),1,fp);
}
void _dump_proc(struct __key_value_node* k_v){
	//struct __key_value_node* pt = k_v_pool->k_v;
	struct __key_value_node* temp;
	int i =0,count = 0;
	int fpos = 0;
	FILE *fp;
	char *num;
	fp = fopen("dict.lib","w+");
	printf("memory dump begin...\n");
	while(k_v && i< KEY_VALUE_SIZE){
		if(k_v[i].key != NULL && k_v[i].value !=NULL){
			printf("key-value:%s->%s\n",k_v[i].key,k_v[i].value);
			count ++;
			__dump(fp,k_v[i].key,k_v[i].value);
			temp = k_v[i].next;
			while(temp->key != NULL && temp->value !=NULL){
				printf("key-value:%s->%s\n",temp->key,temp->value);
				__dump(fp,temp->key,temp->value);
				count ++;
				temp = temp->next;
			}
		}
		i++;
	}
	fclose(fp);
	
	printf("\nmemory dump End... dump count:%d \n",count);
	//_endthreadex(0);
}

void __key_value_dump(struct __key_value_node* k_v){
	unsigned pid;
	_beginthreadex(NULL,0, (unsigned (__stdcall *) (void *))_dump_proc,(void *)k_v,0,&pid);
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
		//printf("read:next->%s\n",readbuf);
		if(!strcmp(readbuf,"1")==0)break;
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
			valuebuf = (char*)malloc(num);
			memset(valuebuf,0,num);
		}
		fread(valuebuf,num,1,fp);
		valuebuf[num]='\0';
		//printf("read:value->%s,strlen:%d\n",valuebuf,strlen(valuebuf));
		__key_value_insert(k_v,keybuf,valuebuf);
		i++;
	}
	while(1);
	fclose(fp);
	printf("recover count:%d\n",i);
}

void* command_parser(char* command){
	char * word, *tmp;
	char* result,*key,*value;
	char *out;cJSON *json, *tmpjson;
	//char * callback_value = ALLOC(char*,sizeof(char*));
	word = strtok(command," ");
	if(strcmp(L(word),"read")==0)
	{
		/*
		 * 读取一个键值对
		 * 语法 read key
		 * 若key不存在则返回 ""k_v_no_value_saved"
		 */
		//memset(callback_value,0,sizeof(char*));
		/*i = atoi(strtok(NULL," "));
		printf("0x%08x\n",callback_value);
		read(i,callback_value);*/
		tmp = strtok(NULL," ");
		result = malloc_and_copy(__key_value_get(key_value,tmp));
		printf("get key:%s,value:%s...\n",tmp,result);
		return result;
		
	}
	if(strcmp(L(word),"write")==0)
	{
		/*  
		 * 写入一个键值对，
		 * 语法 write key value
		 * 如果key已经存在，则替换
		 */
		key = strtok(NULL," ");
		value = strtok(NULL," ");
		if(!value)return "-";
		__key_value_insert(key_value,key,value);
		printf("write:%s:%s\n",key,value);
		return "+";
		/*i = atoi(strtok(NULL," "));
		write(i,strtok(NULL," "));*/
	}
	if(strcmp(L(word),"add")==0){
		/*
		 * 将值加入键值对中
		 * 语法 add key value
		 * * 例如，原先存储的数据为 hello world
		 * * 执行 add hello world2
		 * * 结果 hello->worldworld2
		 */
		key = strtok(NULL," ");
		value = strtok(NULL," ");
		result = malloc_and_copy(__key_value_get(key_value,key));
		tmp = (char*)malloc(strlen(result)+strlen(value));
		sprintf(tmp,"%s%s",result,value);
		__key_value_insert(key_value,key,tmp);
		return "+";
	}
	if(strcmp(L(word),"jsonadd")==0){
		/*
		 * 以json格式添加到键值对中
		*/
		key = strtok(NULL," ");
		value = strtok(NULL," ");
		result = malloc_and_copy(__key_value_get(key_value,key));
		json = cJSON_Parse(result);
		if(!json){
			json = cJSON_CreateArray();
			cJSON_AddItemToArray(json,cJSON_CreateString(result));
		}
		cJSON_AddItemToArray(json,cJSON_CreateString(value));
		result=cJSON_Print(json);
		__key_value_insert(key_value,key,result);
		return "+";

	}
	if(strcmp(L(word),"jsonread")==0){
		/*
		 * 以json格式输出
		 */
		tmp = strtok(NULL," ");
		result = malloc_and_copy(__key_value_get(key_value,tmp));
		json=cJSON_Parse(result);
		if(!json) json =cJSON_CreateString(result);
		result=cJSON_Print(json);
		return result;

	}
	if(strcmp(L(word),"dump")==0){
		__key_value_dump(key_value);
		return "+";
	}
	if(strcmp(L(word),"recvr")==0){
		__recover(key_value);
		return "+";
	}
	return "-";
}