#include"async_serv.h"

#define doExit(...){\
		eprintf(__VA_ARGS__);\
		exit(EXIT_FAILURE);\
}




void serv_thread(struct serv_arguments * arguments){
	
	int ret;

        struct epoll_event ev, events[MAX_LISTEN];
	int epollfd,nfds;



	int main_descriptor = socket(AF_INET, SOCK_STREAM, 0);
	if(main_descriptor == -1) doExit("Can't init socket\n"); 

	struct sockaddr_in my_addr;
	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons( arguments->listen_port );
	my_addr.sin_addr.s_addr = inet_addr(arguments->listen_host);
	
	ret = bind(main_descriptor, (struct sockaddr*)&my_addr, sizeof(struct sockaddr_in));
	if(ret == -1) {
		doExit("Can't init listen on %s:%s\n", arguments->listen_host, arguments->listen_port);
	}
	if( listen(main_descriptor,MAX_LISTEN) == -1) doExit("Cant start listening\n");

/*
	epollfd
*/
	epollfd = epoll_create1(0);
	if (epollfd == -1) {
               perror("epoll_create1");
               exit(EXIT_FAILURE);
        }
	ev.events = EPOLLIN;
	ev.data.fd = main_descriptor;
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, main_descriptor, &ev) == -1) {
               perror("epoll_ctl: main_descriptor");
               exit(EXIT_FAILURE);
        }

	struct sockaddr_in client_addr;
	socklen_t sizeOfSockAddrType = sizeof(struct sockaddr_in);

	for (;;) {//for start
	       //puts("Cycle");
               nfds = epoll_wait(epollfd, events, MAX_LISTEN, -1);
               if (nfds == -1) {
		   puts("Epoll_Wait");
                   perror("epoll_wait");
                   exit(EXIT_FAILURE);
               }

               for (int n = 0; n < nfds; ++n) {
                   if (events[n].data.fd == main_descriptor) {
                       int conn_sock = accept(main_descriptor,
                                          (struct sockaddr *) &client_addr, &sizeOfSockAddrType);
                       if (conn_sock == -1) {
                           perror("accept");
		   	   puts("accept");
                           exit(EXIT_FAILURE);
                       }
		       int flags = fcntl(conn_sock, F_GETFL, 0);
		       fcntl(conn_sock, F_SETFL, flags | O_NONBLOCK);

                       ev.events = EPOLLIN | EPOLLET;
                       ev.data.fd = conn_sock;
                       if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock,
                                   &ev) == -1) {
		   	   puts("epoll_ctl");
                           perror("epoll_ctl: conn_sock");
                           exit(EXIT_FAILURE);
                       }
                   }/*if acceptor*/ else {/*if client*/
		        char buf[1024];
			size_t nbytes;
			const char *ip = inet_ntoa(client_addr.sin_addr);
			const uint16_t port = htons(client_addr.sin_port);
			bzero(buf, sizeof(buf));
			if( (nbytes = recv(events[n].data.fd, buf, sizeof(buf),0)) <= 0){
				if (errno != EWOULDBLOCK && nbytes != 0) {
           			 perror("read/fd2");
        			}
				epoll_ctl(epollfd, EPOLL_CTL_DEL, events[n].data.fd, &ev);
				close(events[n].data.fd);
				continue;
			}
			//printf("%s:%d -> %s\n",ip,port,buf);
			for(int z = sizeof(buf)-1;z--;){
				//printf("buf[%d] = %c\n", z, buf[z]);
				if(buf[z] != 0)
					buf[z] ^= rand() % 10;
			}
			if(send(events[n].data.fd, buf, sizeof(buf), MSG_NOSIGNAL) < 0){
				perror("Write error");
				
			}
                       //do_use_fd(events[n].data.fd);
                   }//else client end
               }//for (events) end
	}//for ;; end
}
