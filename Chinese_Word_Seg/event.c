#include "event.h"



/* global vars */


void event_set(event *ev, int fd,void (*call_back)(int, int, void*), void* arg){
	ev->fd = fd;
	ev->call_back = call_back;
	ev->events = 0;
	ev->arg = arg;
	ev->status = 0;
	ev->last_active = time(NULL);
}

void event_add(int epollFd, int events, event *ev)
{
	struct epoll_event epv = {0, {0}};
	int op;
	epv.data.ptr = ev;
	epv.events = ev->events = events;
	if(ev->status == 1){
		op = EPOLL_CTL_MOD;
	}
	else{
		op = EPOLL_CTL_ADD;
		ev->status = 1;
	}
	if(epoll_ctl(epollFd, op, ev->fd, &epv) < 0)
		printf("Event Add failed[fd=%d]\n", ev->fd);
	//else
	//	printf("Event Add OK[fd=%d]\n", ev->fd);
}
void event_del(int epollFd, event *ev)
{
	struct epoll_event epv = {0, {0}};
	if(ev->status != 1) return;
	epv.data.ptr = ev;
	ev->status = 0;
	epoll_ctl(epollFd, EPOLL_CTL_DEL, ev->fd, &epv);
}
void send_data(int fd,int events,void *arg){
	event *e = (event*)arg;
	int len;
	//send data
	len = send(fd,e->buff,e->len,0);
	e->len = 0;
	event_del(g_epoll_fd,e);
    if(len > 0)
        {
            // change to receive event
           event_set(e, fd, recvfunc, e);
           event_add(g_epoll_fd,EPOLLIN|EPOLLET,e);
        }
        else
        {
            close(e->fd);
            printf("recv[fd=%d] error[%d]/n", fd, errno);
        }
}
void recvfunc(int fd, int events, void *arg){
	event *e = (event*)arg;
	char* result;
	int len;
	len = recv(fd,e->buff,sizeof(e->buff)-1,0);
	event_del(g_epoll_fd,e);
	if(len>0){
		e->len = len;
		e->buff[len] = '\0';
		//printf("C[%d]:%s\n",fd,e->buff );
        		result = recv_callback(e->buff);

        		e-> len = strlen(result);
        		memset(e->buff,0,128);
        		memcpy(e->buff,result,e->len);
               // printf("callback:%s\n", e->buff);
        		//e->buff[len]='\0';
                event_set(e,fd,send_data,e);
                event_add(g_epoll_fd,EPOLLOUT|EPOLLET,e);

	}else if(len==0){
	     printf("[fd=%d] closed gracefully.\n", fd);
		close(e->fd);
	}
	else{
		close(e->fd);
		printf("recv[fd=%d] error[%d]:%s\n", fd, errno, strerror(errno));
	}
}

void accept_conn(int fd, int events, void* arg){
	struct sockaddr_in sin;
	socklen_t len = sizeof(struct sockaddr_in);
	int nfd,i;
	//accept
	if((nfd = accept(fd,(struct sockaddr*)&sin,&len))==-1){
		if(errno != EAGAIN && errno != EINTR){
			printf("%s: bad accept", __func__);
		}
		return;
	}
	do{
		for (i = 0; i < MAX_EVENTS; i++)
		{
			if(g_Events[i].status == 0){
				break;
			}
		}
		if(i==MAX_EVENTS){
			printf("%s:max connection limit %d\n",__func__,MAX_EVENTS );
			break;
		}
		if(fcntl(nfd,F_SETFL,O_NONBLOCK)<0)break;
		event_set(&g_Events[i],nfd,recvfunc,&g_Events[i]);
		event_add(g_epoll_fd,EPOLLIN|EPOLLET,&g_Events[i]);
		printf("new conn[%s:%d] [time:%d]\n", inet_ntoa(sin.sin_addr), ntohs(sin.sin_port), g_Events[i].last_active);

	}while(0);
}

void init_listen_socket(int epoll_fd,int port){
	int listen_fd = socket(AF_INET,SOCK_STREAM,0);
	if(listen == -1){
		printf("call to socket failed.\n");
		exit(1);
	}
	fcntl(listen_fd,F_SETFL,O_NONBLOCK);
	int bReuseaddr=1;
    	setsockopt(listen_fd,SOL_SOCKET ,SO_REUSEADDR,(const char*)&bReuseaddr,sizeof(int));

	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family =AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(port);
	if(bind(listen_fd,(const struct sockaddr*)&sin,sizeof(sin))<0){
		printf("bind error:%d\n",errno );
	}
	if(listen(listen_fd,SOMAXCONN) == -1){
		printf("listen error:%d\n",errno);
	}

	printf("init listen socket.....port:%d, fd=%d\n",port,listen_fd);
	event_set(&g_Events[MAX_EVENTS],listen_fd,accept_conn,&g_Events[MAX_EVENTS]);
	event_add(epoll_fd,EPOLLIN|EPOLLET,&g_Events[MAX_EVENTS]);

}
int init_server(){
	int port = 12345;
	int i;
	g_epoll_fd = epoll_create(MAX_EVENTS);
	if(g_epoll_fd <=0) printf("create epoll failed..\n");
	init_listen_socket(g_epoll_fd,port);
	struct epoll_event events[MAX_EVENTS];
	printf("server running:port[%d]\n",port);
	int check_pos = 0;
	while(1){
		long now = time(NULL);
		for(i=0;i<100;i++,check_pos++){
			if(check_pos == MAX_EVENTS) check_pos=0;
			if(g_Events[check_pos].status !=1) continue;
			long duration = now - g_Events[check_pos].last_active;
			if(duration>=60){
				close(g_Events[check_pos].fd);
				printf("fd=[%d] timeout: [%d,%d]",g_Events[check_pos],g_Events[check_pos].last_active,now);
				event_del(g_epoll_fd,&g_Events[check_pos]);
			}
		}
			//wait for event to happen
		int fds = epoll_wait(g_epoll_fd,events,MAX_EVENTS,1000);
		if(fds<0){
			printf("epoll_wait error:errno:%d;\n",errno);
			if(errno == EINTR){
                continue;
			}else{
                break;
			}
		}
		for(i=0;i<fds;i++){
			event *ev = (event*)events[i].data.ptr;
			 if((events[i].events&EPOLLIN)&&(ev->events&EPOLLIN)) // read event
			 {
			 	ev->call_back(ev->fd, events[i].events, ev->arg);
			 }
            if((events[i].events&EPOLLOUT)&&(ev->events&EPOLLOUT)) // write event
            {
            	ev->call_back(ev->fd, events[i].events, ev->arg);
            }
        }
    }
return 1;
}

void set_user_recv_callback(char*(*callback)(char*)){
	user_recv_callback   = callback;
}

char* recv_callback(char* str){
	if(user_recv_callback){
		//user_recv_callback defined
		//printf("function defined\n");
		return  user_recv_callback(str);
		//printf("retval:%s\n", retval);
	}else{
		//user_recv_callback_undefined:
		return str;

	}
}
