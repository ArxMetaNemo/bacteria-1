#include<time.h>
#include"cryptocoins.h"
#include"signal_handler.h"
#include<pthread.h>
#include"async_serv.h"

//void __start(void){} #

int main(int argc, char ** argv){
	catch_badsignals();
	srand(time(NULL));
	if(argc != 3) return eprintf("%s host port\n", argv[0], argv[1], argv[2]);

	struct cryptocoin * cryptocoins = init_cryptocoins("cryptocoins.ini");
	dump_cryptocoins(cryptocoins);


	/*
		server part
	*/
	pthread_t pthreadServ;
	struct serv_arguments args = {argv[1],atoi(argv[2]), cryptocoins};
	void * (*fun)(void *)=(void*)serv_thread;

	if ( pthread_create(&pthreadServ, NULL, fun, (void*)&args) != 0){
		return eprintf("can't start thread!\n");
	}
	
	pthread_join(pthreadServ,NULL);

	clear_cryptocoins(cryptocoins);


}
