#include"signal_handler.h"
#include<stdlib.h>
#include<stdio.h>
void sigpipe(int signum){
	//todo: mechanism or to SIGIGN
}

void close_program(int signum){
	puts("bye!");
	exit(0);
}

void catch_badsignals(void){
	signal(SIGPIPE, sigpipe);
	signal(SIGINT, close_program);
}
