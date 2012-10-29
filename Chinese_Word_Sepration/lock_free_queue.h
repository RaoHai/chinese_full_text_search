#include <stdio.h>
#include <Windows.h>

#define TRUE 1
#define FALSE 0
#define ERR_EMPTY_QUEUE NULL
#define BOOL int


struct file_node
{
	int file_pos;
	char* value;
	struct file_node *next;
};

struct lock_free_queue
{
	struct file_node *front;
	struct file_node *tail;
}file_write_queue;


BOOL CAS(struct file_node** , struct file_node*,struct file_node*);
void init_queue(struct file_node* new_node);
void en_queue(int filepos,char* value);
struct file_node* out_queue();
int queue_is_empty();
void clear_queue();
void trace_queue();
