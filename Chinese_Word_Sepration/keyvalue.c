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
	char * newstr = (char*)sub_allocate(strlen(source));
	memset(newstr,0,strlen(source)*sizeof(char));
	strcpy(newstr,source);
	return newstr;
}
struct __key_value_node*  __key_value_init()
{
	for(;;)
	{
		struct __key_value_node * k_v ;
		_k_v_pool_init();
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
	//FILE *fp;
	//long filepos = (long)hashed;
	struct __key_value_node *newnode;
	struct __key_value_node *iter =  &key_value[hashed];
	//fp= fopen("dict.lib","rb+");
	//fseek(fp,filepos,SEEK_SET);
	while(iter&&iter->next!=NULL)
	{
		if(strcmp(iter->key,key)==0) break;
		iter = iter->next;
	}

	if(!iter->key || !iter->next){
		//newnode = get_from_pool(1);
		newnode = (struct __key_value_node *)sub_allocate(sizeof(struct __key_value_node));
		memset(newnode,0,sizeof(struct __key_value_node));
		iter->next = newnode;
		nextpos = newnode->pos;
	}else{
		nextpos = iter->next->pos;
	}
	if(iter->pos && iter->pos > KEY_VALUE_SIZE) filepos = iter->pos;
	
	iter->value = malloc_and_copy(value);
	iter->key = malloc_and_copy((char*)key);
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


int _k_v_pool_init(){
	for(;;)
	{
		
		//k_v_pool = (struct _k_v_pool* )sub_allocate(sizeof(struct _k_v_pool));
		k_v_pool = (struct _k_v_pool* ) malloc (sizeof(struct _k_v_pool));
		k_v_pool->k_v = (struct __key_value_node*) malloc (KEY_VALUE_MUTIPLUS * KEY_VALUE_SIZE * sizeof(struct __key_value_node));
		if (k_v_pool->k_v) 
		{
			memset(k_v_pool->k_v,0,KEY_VALUE_MUTIPLUS * KEY_VALUE_SIZE*sizeof(struct __key_value_node));
			break;
		}
	}
	/*
	fp= fopen("dict.lib","wb+");
	fwrite(k_v_pool->k_v,sizeof(struct __key_value_node),5 * KEY_VALUE_SIZE,fp);
	//*/
	k_v_pool->pt = k_v_pool->k_v;
	k_v_pool->pool_deep = KEY_VALUE_MUTIPLUS * KEY_VALUE_SIZE;
	k_v_pool->water_deep = 0;
	printf("k_v_pool init successed!\n");
	return 1;
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
void _dump_proc(){
	struct __key_value_node* pt = k_v_pool->k_v;
	int i =0;
	int fpos = 0;
	FILE *fp;
	char *num;
	fp = fopen("dict.lib","w+");
	printf("memory dump begin...\n");
	while(pt && i < k_v_pool->pool_deep){
		if(pt[i].key){
			//printf("1%d%s%d%s",strlen(pt[i].key),pt[i].key,strlen(pt[i].value),pt[i].value);
			num = (char*)malloc(sizeof(char)*sizeof(int));
			//itoa(1,num,10);
			fwrite("1",sizeof(int),1,fp);
			
			itoa(strlen(pt[i].key),num,10);
			fwrite(num,sizeof(int),1,fp);
			
			fwrite(pt[i].key,strlen(pt[i].key),1,fp);
			//fwrite('\0',sizeof(char),1,fp);
			itoa(strlen(pt[i].value),num,10);
			fwrite(num,sizeof(int),1,fp);

			fwrite(pt[i].value,strlen(pt[i].value),1,fp);
			//fwrite('\0',sizeof(char),1,fp);
		}
		i++;
	}
	fclose(fp);
	printf("\nmemory dump End...\n");
	//_endthreadex(0);
}

void __key_value_dump(){
	unsigned pid;
	_beginthreadex(NULL,0, (unsigned (__stdcall *) (void *))_dump_proc,(void *)0,0,&pid);
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
	do{
		readbuf = (char*)realloc(readbuf,sizeof(int));
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

void _mem_pool_trace()
{
	struct __key_value_node* pt = k_v_pool->k_v;
	int i=0;

	while(pt&& i< k_v_pool->pool_deep)
	{
		if(pt[i].key)
			printf(" %d:%s ",i,pt[i].key);
		
		i++;
	}
	
}

/*
	自动化测试
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
