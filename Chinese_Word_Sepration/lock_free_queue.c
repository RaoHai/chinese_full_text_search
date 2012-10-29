#include "lock_free_queue.h"
/*
 * windows 的原生CAS不太好用
*/
BOOL CAS(struct file_node** reg, struct file_node* oldval,struct file_node* newval)
{
	if(*reg == oldval)
	{
		*reg = newval;

		return TRUE;
	}
	return FALSE;
}
void init_queue(struct file_node* new_node)
{
	file_write_queue.front = file_write_queue.tail = new_node;
}

void en_queue(int filepos,char* value)
{
	struct file_node* new_node,*p,*oldp;
	
	new_node = (struct file_node*)malloc(sizeof(struct file_node*));
	new_node->file_pos = filepos;
	new_node->value = value;
	new_node->next = NULL;
	p = file_write_queue.tail;
	if(!p)
	{
		init_queue(new_node);
	}
	else
	{
		oldp= p;
		printf("waiting for CAS...");
		do
		{
			 while (p->next != NULL)
			   p = p->next;
		}while( CAS(&p->next, NULL, new_node) != TRUE); 
		printf("add to queue:%d:%s\n",filepos,value);
		CAS(&file_write_queue.tail, oldp, new_node);
		if(queue_is_empty())file_write_queue.front =file_write_queue.tail;
	}

}
struct file_node* out_queue()
{
	struct file_node* p ;
	do
	{
		p=  file_write_queue.front;
		if(p->next==NULL)
			return ERR_EMPTY_QUEUE;
	}while(CAS(&file_write_queue.front,p,p->next));
	return p->next;
}
int queue_is_empty()
{
	return file_write_queue.front == NULL;
}
void trace_queue(){
	struct file_node *p= file_write_queue.front;
	printf("trace queue:-----------------------\n");
	printf("queue front:%d:%s\n",p->file_pos,p->value);
	printf("queue tail:%d:%s\n",file_write_queue.tail->file_pos,file_write_queue.tail->value);
	do
	{
		printf("trace queue:%d:%s\n",p->file_pos,p->value);
		p=p->next;
	}while(p);
	printf("-----------------------trace queue end\n");
}

