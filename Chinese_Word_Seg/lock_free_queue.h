#include <stdio.h>
//#include <Windows.h>

#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#define TRUE 1
#define FALSE 0
#define ERR_EMPTY_QUEUE NULL
#define BOOL int


struct file_node
{
	int file_pos;
	void* value;
	struct file_node *next;
};

struct lock_free_queue
{
	struct file_node *front;
	struct file_node *tail;
};


BOOL CAS(struct file_node** , struct file_node*,struct file_node*);
struct lock_free_queue* new_lock_free_queue();
void init_queue(struct lock_free_queue*,struct file_node* new_node);
void en_queue(struct lock_free_queue*,int filepos,void* value);
void* out_queue(struct lock_free_queue*);
int queue_is_empty(struct lock_free_queue*);
void clear_queue(struct lock_free_queue*);
void trace_queue(struct lock_free_queue*);

#endif/*QUEUE_H_INCLUDED*/
