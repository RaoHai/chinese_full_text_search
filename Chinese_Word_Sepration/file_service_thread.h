#include "lock_free_queue.h"

#ifndef LOCK_FREE_H_INCLUDED
#define LOCK_FREE_H_INCLUDED

struct _file_service
{
	struct lock_free_queue *write_queen, *read_queen;

}file_service;

struct _file_struct
{
	int next;
	char key[20];
	char value[20];
	
};

int init_file_service();
void service_proc();
void* read(int filepos,void* callback_value);
void  write(int,char*,char*,int);
int restruct_key_value();

#endif