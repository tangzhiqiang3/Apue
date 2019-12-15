#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<getopt.h>
#include<stdlib.h>
//#define SERVER_IP    	"127.0.0.1"
//#define LISTEN_PORT	8889
//#define BACKLOG		13
#define MSG_STR		"Hello,Lingyun IOT Studio Client\n"

void print_usage(char *progname)
{
	printf("%s usage: \n", progname);
	printf("-p(--port): sepcify server listen port.\n");
	printf("-h(--Help): print this help information.\n");

	return ;
}

int main (int argc, char **argv)
{
  	int			rv = -1;
  	int			listen_fd, client_fd = -1;
  	struct sockaddr_in	serv_addr;
  	struct sockaddr_in    	cli_addr;
 	socklen_t		cliaddr_len;
  	char			buf[1024];
  	int			on = 1;
	int 			ch;
	int 			port = 0;
	struct option lopts[] = {
	{"port", required_argument, NULL, 'p'},
	{"help", no_argument, NULL, 'h'},
	{NULL, 0, NULL, 0}
};   
	pid_t 			pid;	
 
while((ch = getopt_long(argc, argv, "p:h", lopts, NULL)) != -1)
{
	switch(ch)
	{
		case 'p':
			port=atoi(optarg);
			break;
		case 'h':
			print_usage(argv[0]);
			return 0;
	}
}
if(!port)
{
	print_usage(argv[0]);
}	
listen_fd = socket(AF_INET, SOCK_STREAM, 0);
if(listen_fd < 0)
{
	printf("create socket failure: %s\n",strerror(errno));
    	return -1;
}
printf("create socket fd:[%d]\n",listen_fd);

setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
  
memset(&serv_addr, 0, sizeof(serv_addr));
serv_addr.sin_family = AF_INET;
serv_addr.sin_port = htons(port);
serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
//inet_aton("192.168.0.16", &servaddr.sin_addr);

if(bind(listen_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 )
{
	printf("create socket failure: %s\n",strerror(errno));
    	return -2;
}    
printf("socket[%d] bind on port[%d] for all IP address ok\n",listen_fd, port);

listen(listen_fd, 13);
printf("Start to listen on port [%d]\n",port);

while(1) 
{
	printf("\nStart waiting and accept new client connect...\n");
    	client_fd = accept(listen_fd, (struct sockaddr *)&cli_addr, &cliaddr_len);
    	if(client_fd < 0)
    	{
      		printf("accept new socket failure: %s\n",strerror(errno));
      		return -2;
    	}
    	printf("Accept new client[%s:%d] with fd [%d]\n",inet_ntoa(cli_addr.sin_addr),ntohs(cli_addr.sin_port), client_fd);    //IP地址的字符串，端口号，fd
	
	pid=fork();
	if(pid < 0)
	{
		printf("fork() create failure: %s\n",strerror(errno));
		close(client_fd);
		continue;
	}
	else if(pid < 0)		//父进程执行程序
	{
		close(client_fd);
		continue;
	}
	else if(pid == 0) 
	{	
		char	buf[1024];
		close(listen_fd); 	//子进程关闭套接字 
		printf("Child process start execute program...\n");
    		memset(buf, 0, sizeof(buf));

    		if((rv = read(client_fd, buf, sizeof(buf))) < 0)
    		{
      			printf("Read data from client socket [%d] fialure: %s\n",client_fd, strerror(errno));
      			// return -2;
      			close(client_fd);
      			exit(0);
   		}
    		else if(rv == 0)
    		{
      			printf("client socket [%d] disconnected\n",client_fd);
      			close(client_fd);
      			exit(0);
    		}
    		printf("read %d byte data from client [%d] and echo it back: '%s'\n",rv, client_fd, buf);
    
    		if(write(client_fd, MSG_STR, strlen(MSG_STR)) < 0)
    		{
      			printf("Write %d byte data back to client [%d] fialure: %s\n",rv, client_fd, strerror(errno));
     	 		close(client_fd);
			exit(0);  
		}
    
    	sleep(1);
	printf("Close client socket[%d] and child process exit\n",client_fd);
    	close(client_fd);
	}
}
close(listen_fd);
return 0;
}


