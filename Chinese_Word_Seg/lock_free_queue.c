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
struct lock_free_queue* new_lock_free_queue()
{
	struct lock_free_queue * new_queue; 
	for(;;)
	{
				new_queue= (struct lock_free_queue *)malloc(sizeof(struct lock_free_queue *));
				if(new_queue)
				{
					memset(new_queue,0,sizeof(struct lock_free_queue *));
					break;
				}
	}
	
	new_queue->front = new_queue->tail=NULL;
	return new_queue;
}
void init_queue(struct lock_free_queue* queue,struct file_node* new_node)
{

	queue->front = new_node;
	queue->tail = new_node;

	//file_write_queue.front = file_write_queue.tail = new_node;
}

void en_queue(struct lock_free_queue* queue,int filepos,void* value)
{
	struct file_node* new_node,*p,*oldp;
	
	new_node = (struct file_node*)malloc(sizeof(struct file_node));
	memset(new_node,0,sizeof(struct file_node));
	new_node->file_pos = filepos;
	new_node->value = value;
	new_node->next = NULL;
	p = queue->tail;

	if(!p)
	{
		init_queue(queue,new_node);
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
		
		CAS(&queue->tail, oldp, new_node);
		if(queue_is_empty(queue))queue->front =queue->tail;
	}

}
void* out_queue(struct lock_free_queue* queue)
{
	struct file_node* p ;
	do
	{
		p=  queue->front;

		if(p == NULL)
			return ERR_EMPTY_QUEUE;
	}while(CAS(&queue->front,p,p->next)!=TRUE);

	return p;
}
int queue_is_empty(struct lock_free_queue* queue)
{

	return queue->front == NULL;
}
void trace_queue(struct lock_free_queue* queue){
	struct file_node *p= queue->front;
	printf("trace queue:-----------------------\n");
	printf("queue front:%d:%s\n",p->file_pos,p->value);
	printf("queue tail:%d:%s\n",queue->tail->file_pos,queue->tail->value);
	do
	{
		printf("trace queue:%d:%s\n",p->file_pos,p->value);
		p=p->next;
	}while(p);
	printf("-----------------------trace queue end\n");
}

