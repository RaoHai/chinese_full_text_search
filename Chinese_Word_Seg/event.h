#ifndef EVENT_H_INCLUDED

#define EVENT_H_INCLUDED

#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>


#define MAX_EVENTS 2048
typedef struct event
{
    int fd;
    void (*call_back)(int fd, int events, void *arg);
    int events;
    void *arg;
    int status; // 1: in epoll wait list, 0 not in
    char buff[128]; // recv data buffer
    int len;
    long last_active; // last active time
} event;

int g_epoll_fd;
event g_Events[MAX_EVENTS+1];

void recvfunc();
void event_set(event *ev, int fd,void (*call_back)(int, int, void*), void* arg);
void event_add(int epollFd, int events, event *ev);
void event_del(int epollFd, event *ev);
void send_data(int fd,int events,void *arg);
void recvfunc(int fd, int events, void *arg);
void accept_conn(int fd, int events, void* arg);
void init_listen_socket(int epoll_fd,int port);
int init_server();


char* recv_callback(char *);
char* (*user_recv_callback)(char *);
void set_user_recv_callback(char*(*user_recv_callback)(char*));
/*Prototypes*/

#endif // EVENT_H_INCLUDED
