#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>



#include "trie_tree.h"


#include "libevent/event2/buffer.h"
#include "libevent/event2/bufferevent.h"
#include "libevent/event2/listener.h"


#ifndef KEY_VALUE_TCP_H
#define KEY_VALUE_TCP_H

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
		//printf("read line from buffer:%s\n",line);
		retval = (char*)command_parser(line);
		//command_parser(line);
		evbuffer_reserve_space(output, strlen(retval), v, 2);
		evbuffer_add(output, retval, strlen(retval));
		evbuffer_commit_space(output, v, 1);

		//evbuffer_add_buffer(output, input);


		//evbuffer_copyout(input, line, buffer_len);

        /* Copy all the data from the input buffer to the output buffer. */
		//printf("%s\n",line);
		//free(retval);
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

int tcp_start_up();

#endif
