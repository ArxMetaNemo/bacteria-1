#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>          /* See NOTES */
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<unistd.h>
#include<time.h>
#include<string.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include"macros.h"
struct serv_arguments{
	const char * listen_host;
	uint16_t listen_port;
	struct cryptocoin * cryptocoins;
};

void serv_thread(struct serv_arguments * arguments);
