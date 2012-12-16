#include <stdio.h>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#define   _WIN32_WINNT   0x0500

#include <Windows.h>
#include <WinSock2.h>

#include "keyvalue.h"


#include "libevent\event2\buffer.h"
#include "libevent\event2\bufferevent.h"
#include "libevent\event2\listener.h"


#ifndef KEY_VALUE_TCP_H
#define KEY_VALUE_TCP_H

static void echo_read_cb(struct buffevent *bev,void* ctx);
static void echo_event_cb(struct bufferevent *bev, short events, void *ctx);
static void accept_conn_cb(struct evconnlistener *listener, evutil_socket_t fd, 
							struct sockaddr *address, int socklen,  void *ctx);
static void accept_error_cb(struct evconnlistener *listener, void *ctx);

int tcp_start_up();

#endif