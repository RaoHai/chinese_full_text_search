#include "key_value_tcp.h"


static void echo_read_cb(struct bufferevent *bev, void *ctx)
{
	
		char * line;
		char* retval;
		size_t len;
        /* This callback is invoked when there is data to read on bev. */
        struct evbuffer *input = bufferevent_get_input(bev);
        struct evbuffer *output = bufferevent_get_output(bev);
		struct evbuffer_iovec v[2];

		line = evbuffer_readln(input, &len, EVBUFFER_EOL_CRLF);
		retval = (char*)command_parser(line);
		//command_parser(line);
		evbuffer_reserve_space(output, strlen(retval), v, 2);
		evbuffer_add(output, retval, strlen(retval));
		evbuffer_commit_space(output, v, 1);
		
		//evbuffer_add_buffer(output, input);
		

		//evbuffer_copyout(input, line, buffer_len);
		
        /* Copy all the data from the input buffer to the output buffer. */
		printf("%s\n",line);
		free(line);    
}
 
static void echo_event_cb(struct bufferevent *bev, short events, void *ctx)
{
        if (events & BEV_EVENT_ERROR)
                perror("Error from bufferevent");
        if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
                bufferevent_free(bev);
        }
}
 
static void accept_conn_cb(struct evconnlistener *listener,
    evutil_socket_t fd, struct sockaddr *address, int socklen,
    void *ctx)
{
        /* We got a new connection! Set up a bufferevent for it. */
        struct event_base *base = evconnlistener_get_base(listener);
        struct bufferevent *bev = bufferevent_socket_new(
                base, fd, BEV_OPT_CLOSE_ON_FREE);
 
        bufferevent_setcb(bev, echo_read_cb, NULL, echo_event_cb, NULL);
        bufferevent_enable(bev, EV_READ|EV_WRITE);
}
 
static void accept_error_cb(struct evconnlistener *listener, void *ctx)
{
        struct event_base *base = evconnlistener_get_base(listener);
        int err = EVUTIL_SOCKET_ERROR();
        fprintf(stderr, "Got an error %d (%s) on the listener. "
                "Shutting down./n", err, evutil_socket_error_to_string(err));
 
        event_base_loopexit(base, NULL);
}

int tcp_start_up(){

	WSADATA WSAData;

	struct event_base *base;
    struct evconnlistener *listener;
    struct sockaddr_in sin;

    int port = 22222;
	WSAStartup(0x101, &WSAData);
      
    base = event_base_new();
    if (!base) {
            puts("Couldn't open event base");
            return 1;
    }

    /* Clear the sockaddr before using it, in case there are extra
        * platform-specific fields that can mess us up. */
    memset(&sin, 0, sizeof(sin));
    /* This is an INET address */
    sin.sin_family = AF_INET;
    /* Listen on 0.0.0.0 */
    sin.sin_addr.s_addr = htonl(0);
    /* Listen on the given port. */
    sin.sin_port = htons(port);

    listener = evconnlistener_new_bind(base, accept_conn_cb, NULL,
        LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE, -1,
        (struct sockaddr*)&sin, sizeof(sin));
    if (!listener) {
            perror("Couldn't create listener");
            return 1;
    }

    evconnlistener_set_error_cb(listener, accept_error_cb);
	printf("ready to dispatch...\n");
    event_base_dispatch(base);
	
    return 0;
}