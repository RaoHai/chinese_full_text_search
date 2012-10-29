#include <stdio.h>
#include <WinSock2.h>
#pragma   comment(lib, "ws2_32.lib ")

#define STR_SIZE 2000

void _tcp_int_and_listen(int port,void callback(char*));