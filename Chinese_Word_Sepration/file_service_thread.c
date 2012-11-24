#include <process.h>

#include "file_service_thread.h"
#include "keyvalue.h"


int init_file_service()
{
	unsigned pid;
	char *blank = "";
	FILE* fp;
	struct _file_struct *fs ;
	file_service.write_queen = new_lock_free_queue();
	file_service.read_queen = new_lock_free_queue();

	_beginthreadex(NULL,0, (unsigned (__stdcall *) (void *))service_proc,(void *)0,0,&pid);

	fp = fopen("dict.lib","r");
	if(fp) {
		fclose(fp);
		restruct_key_value();
	}else{
		printf("creating dict file...\n");
		fs = (struct _file_struct*) malloc (KEY_VALUE_MUTIPLUS * KEY_VALUE_SIZE * sizeof(struct _file_struct));
		memset(fs,0,KEY_VALUE_MUTIPLUS * KEY_VALUE_SIZE * sizeof(struct _file_struct));
		fp = fopen("dict.lib","wb+");
		fwrite(fs,sizeof(struct _file_struct),KEY_VALUE_MUTIPLUS * KEY_VALUE_SIZE,fp);
		fclose(fp);
	}

	/*
	memset(fs,0,5 * KEY_VALUE_SIZE * sizeof(struct _file_struct));
	fp = fopen("dict.lib","wb+");
	fwrite(fs,sizeof(struct _file_struct),5 * KEY_VALUE_SIZE,fp);
	fclose(fp);
	//*/
	return 1;
}
int restruct_key_value(){
	FILE *fp;
	struct __key_value_node *pt = k_v_pool->k_v;
	struct __key_value_node *tp;
	int i=0;
	int filepos;
	struct _file_struct* fp_read = (struct _file_struct*)malloc(sizeof(struct _file_struct));
	memset(fp_read,0,sizeof(struct _file_struct *));
	fp = fopen("dict.lib","r");
	if(fp) {
		for(i=0;i<KEY_VALUE_MUTIPLUS * KEY_VALUE_SIZE;i++){
			filepos = i * sizeof(struct _file_struct);
			fseek(fp,filepos,SEEK_SET);
			fread(fp_read,sizeof(struct _file_struct),1,fp);  
			if(strlen(fp_read->key)>0)
			{
				/*
				pt->key =  (char*)malloc(sizeof(char)*strlen(fp_read->key));
				pt->value  =  (char*)malloc(sizeof(char)*strlen(fp_read->value));
				
				strcpy(pt->key,fp_read->key);
				strcpy(pt->value,fp_read->value);
				*/
				pt->key = malloc_and_copy(fp_read->key);
				pt->value = malloc_and_copy(fp_read->value);
				if(fp_read->next){
					tp = k_v_pool->k_v;
					tp +=fp_read->next;
					pt->next =tp;
				}
				printf("restruct: %s -> %s \n",pt->key,pt->value);
			}
			++pt;

		}
	}
	else{
		printf("read file failed..\n");
	}
	fclose(fp);
}
void service_proc()
{
	struct file_node* tmp;
	char* retval;
	int filepos;
	int i;
	FILE *fp;
	struct _file_struct *fs;
	char *temp = (char*)malloc(sizeof(char*));
	char *changed="changed value";
	
	for(;;){
		if(!queue_is_empty(file_service.read_queen))
		{
			tmp = (struct file_node* )out_queue(file_service.read_queen);
			if(tmp==ERR_EMPTY_QUEUE) printf("ERROR:EMPTY QUEUE\n");
			retval = (char*)tmp->value;
			
			filepos =  tmp->file_pos * sizeof(struct _file_struct);
			fp = fopen("dict.lib","rb+");
			
			fseek(fp,filepos,SEEK_SET);
			fs = (struct _file_struct*)malloc(sizeof(struct _file_struct));
			i = fread(fs,sizeof(struct _file_struct),1,fp);                   
			printf("->Read file: filepos:%d,filevalue:%s \n",tmp->file_pos,fs->value);
			//printf("out_queue:0x%08x \n",retval);
			memset(retval,0,sizeof(char*));
			changed = fs->value;
			if(strlen(changed)==0) changed=NO_VALUE;
			strcpy(retval,changed);
			//printf("retval changed:0x%08x \n",retval);
			//strcpy(retval,changed);
			fclose(fp);
			
		}
		else
		{
			if(!queue_is_empty(file_service.write_queen))
			{
				
				//TODO: FILE WRITE ACTION
				
				tmp = (struct file_node*)out_queue(file_service.write_queen);
				filepos =  tmp->file_pos * sizeof(struct _file_struct);
				
				fs = (struct _file_struct*)tmp->value;
				if(tmp->file_pos >KEY_VALUE_SIZE) printf(">%d:%s ===> %s\n",KEY_VALUE_SIZE,fs->key,fs->value);
				//printf("->Write file: filepos:%d,filevalue:%s \n",tmp->file_pos,fs->value);

				/*fs = (struct _file_struct*)malloc(sizeof(struct _file_struct));
				retval = (char*)malloc(sizeof(char*));
				itoa(tmp->file_pos,retval,10);
				strcpy(fs->key,retval);
				strcpy(fs->value,(char*)tmp->value);
				if(tmp->file_pos)	fs->next = tmp->file_pos;
				*/
				///*
				fp = fopen("dict.lib","rb+");
				fseek(fp,filepos,SEEK_SET);
				fwrite(fs,sizeof(struct _file_struct),1,fp);
				fclose(fp);
				//*/


			}
		}
	}
}
void* read(int filepos,void* callback_value)
{
	printf("in read function: 0x%08x\n",callback_value);
	en_queue(file_service.read_queen,filepos,callback_value);
	///*
	
	for(;;)
	{
		if(strcmp((char*)callback_value,"")!=0)
		{
			printf("value changed!: %s\n",callback_value);
			return callback_value;
			break;
		}
	}
	//*/

}
void write(int filepos,char* key,char* value,int next)
{
	struct _file_struct* tmp = (struct _file_struct*)malloc(sizeof(struct _file_struct));
	memset(tmp,0,sizeof(struct _file_struct));
	//char *retval = (char*)malloc(sizeof(char*));
	//itoa(filepos,retval,10);
	strcpy(tmp->key,key);
	strcpy(tmp->value,value);
	tmp->next = next;
	
	en_queue(file_service.write_queen,filepos,tmp);
}
