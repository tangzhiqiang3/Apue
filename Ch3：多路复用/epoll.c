#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<ctype.h>
#include<time.h>
#include<pthread.h>
#include<getopt.h>
#include<libgen.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include<netinet/in.h>
#include<sys/resource.h>

#define MAX_EVENTS  	512
#define ARRAY_SIZE9(x)	(sizeof(x)/sizeof(x[0]))	

static inline void print_usage(char *progname);
int socket_server_init(char *listen_ip, int listenfd);
void set_socket_rlimit(void);

int main(int argc, char **argv)
{
	int 	listenfd, connfd;
	int 	rv;
	int 	daemon_run = 0;
	int 	serv_port = 0;
	char	*progname;
	int 	opt;
	int	i, j;
	int	found;
	char	buf[1024];
	
	int 	epollfd;
	struct epoll_event 	event;
	struct epoll_event 	event_array[MAX_EVENTS];
	int 	events;		//存放epoll_wait()的返回值	

	struct option long_options[] ={
	{"daemon", no_argument, NULL, 'b'},
	{"port", required_argument, NULL, 'p'},
	{"help", no_argument, NULL, 'h'},
	{NULL, 0, NULL, 0}
	};
	
	progname = basename(argv[0]);

	while((opt = getopt_long(argc, argv, "bp:h", long_options, NULL)) != -1)
	{
	    switch(opt)
	    {
		case 'b':
			daemon_run = 1;break;
		case 'p':
			serv_port = atoi(optarg);break;
		case 'h':
			print_usage(progname);break;
		return EXIT_SUCCESS;
		default: break;
	    }
	}
	
	if(!serv_port)
	{
		print_usage(progname);
		return -1;
	}
	
	if((listenfd=socket_server_init(NULL, serv_port)) < 0)
	{
		printf("ERROR: %s socket listen failure on port:%d\n",argv[0], serv_port);
		return -2;
	}
	printf("listen port:[%d] successfully\n", serv_port);
	
	if(daemon_run)
	{
		daemon(0,0);
	}

	/* 将加入listenfd 到兴趣数组中 */
	if((epollfd = epoll_create(MAX_EVENTS)) < 0)
	{
		printf("epoll create failure: %s\n", strerror(errno));
		return -3;
	}
	event.events = EPOLLIN;
	event.data.fd = listenfd;
	
	if(epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &event) < 0)
	{
		printf("epoll add listen socket fialure: %s\n",strerror(errno));
		return -4;
	}
	for( ; ; )
	{
		events=epoll_wait(epollfd, event_array, MAX_EVENTS, -1);
		if(events < 0)
		{
			printf("epoll error: %s\n",strerror(errno));
			break;
		}
		if(events==0)
		{
			printf("epoll get timeout\n");
			break;
		}
		/* events > 0 is the active events count */
		for(i=0; i<events; i++)
		{
			if((event_array[i].events&EPOLLERR) || (event_array[i].events&EPOLLHUP))
			{
				printf("epoll_wait get error on fd[%d]: %s\n",event_array[i].data.fd, strerror(errno));
				epoll_ctl(epollfd, EPOLL_CTL_DEL, event_array[i].data.fd, NULL);
				close(event_array[i].data.fd);
			}
			/* listen socket get event means new client start connect now */
			if(event_array[i].data.fd==listenfd)
			{
				if((connfd=accept(listenfd, (struct sockaddr *)NULL, NULL)) < 0)
				{
					printf("accept socket connfd failure: %s\n",strerror(errno));
					//epoll_ctl(epollfd, EPOLL_CTL_DEL, event_array[i].data.fd, NULL);
					//close(event_array[i].data.fd);
					continue;
				}
				event.events = EPOLLIN;
	         		event_array[i].data.fd = connfd;
				
				if(epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &event) < 0)
				{
					printf("epoll add connfd socket failure: %s\n",strerror(errno));
					close(event_array[i].data.fd);
					continue;
				}
				printf("epoll add new client socket[%d] ok\n", event_array[i].data.fd);
			}
			else /* data arrive from already connected client */
			{
				if((rv=read(event_array[i].data.fd, buf, sizeof(buf))) < 0)
				{
					printf("read from already connected client failure: %s", strerror(errno));
					epoll_ctl(epollfd, EPOLL_CTL_DEL ,event_array[i].data.fd, NULL);
					close(event_array[i].data.fd);
					continue;
				}
				else
				{
					printf("read client %d byte data from fd: %d\n",rv, event_array[i].data.fd);
				
					for(j=0;j<rv;j++)
					{
						buf[j]=toupper(buf[j]);
					}
				
					if(write(event_array[i].data.fd, buf, rv) < 0)
					{
						printf("socket[%d] write failure: %s", event_array[i].data.fd, strerror(errno));
						epoll_ctl(epollfd, EPOLL_CTL_DEL, event_array[i].data.fd, NULL);		
						close(event_array[i].data.fd);
					}
				}
			}//data arrive from already connected client
		}//for(i=0;i<event;i++)
	}//for(; ;)
CleanUp:
	close(listenfd);
	return 0;
}//main()


static inline void print_usage(char *progname)
{
	printf("Usage: %s [option]...\n", progname);
	printf("%s is a socket server program, which used to verity client and echo back string from it\n",progname);
	printf("\nMandatory arguments to long options are mandatory for short options too:\n");
	printf("-b[daemon] set program running on background\n");
	printf("-p[port] Socket server port address\n");
	printf("-h[help] Example: %s -b -p 8989\n",progname);
	return ;
}


int socket_server_init(char *listen_ip, int listen_port)
{
	struct sockaddr_in servaddr;
	int rv = 0;
	int on = 1;
	int listenfd;
	
	if((listenfd=socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("Use socket() to create a TCP socket failure: %s\n",strerror(errno));
		return -1;
	}
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(listen_port);

	if(!listen_ip)	/* Listen all the local IP address */
	{
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else /* listen the specified IP address */
	{
		if(inet_pton(AF_INET, listen_ip, &servaddr.sin_addr) <= 0)
		{
			printf("inet_pton() set listen IP address failure.\n");
			rv = -2;
			goto CleanUp;
		}
	}
	if((bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr))) < 0)
	{
		printf("Use bind() to bind the TCP socket failure: %s\n", strerror(errno));
		rv = -3;
		goto CleanUp;
	}
	if(listen(listenfd, 64) < 0)
	{
		printf("Use listen() to listen the TCP socket failure: %s\n",strerror(errno));
		rv = -4;
		goto CleanUp;
	}
CleanUp:
	if(rv < 0) close(listenfd);
	else 
	rv = listenfd;
	return rv;
}

/* Set open file desciption count to max */
void set_socket_rlimit(void)
{
	struct rlimit limit = {0};

	getrlimit(RLIMIT_NOFILE, &limit);
	limit.rlim_cur = limit.rlim_max;
	setrlimit(RLIMIT_NOFILE, &limit);
	
	printf("set socket open fd max count to %d\n",limit.rlim_max);
}

