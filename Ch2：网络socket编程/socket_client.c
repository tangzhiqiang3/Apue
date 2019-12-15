#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <getopt.h>
#include <stdlib.h>

//#define SERVER_PORT               12345
#define MSG_STR 	     "Hello, Unix Network Program World!"
//#define SERVER_IP 	     "192.168.174.1"	

void print_usage(char *progname)
{
	printf("%s usage: \n",progname);
	printf("-i(--ipaddr): sepcify server IP address.\n");
	printf("-p(--port): sepcify server IP port.\n");
	printf("-h(--help): print this help information.\n");

	return ;		//void类型，无返回值
}

int main(int argc, char **argv)
{
   	int                     conn_fd = -1;
    	struct sockaddr_in      serv_addr;             
	//socklen_t               cliaddr_len;  //在32/64位主机下，该数据类型仍是4字节
    	char                    buf[1024]; 
   	int			rv = -1; 
	char			*SERVER_IP = NULL;
	int			SERVER_PORT = 0;   
	int 			opt;
	struct option lopts[] = {
		{"ipaddr", required_argument, NULL, 'i'},
		{"port", required_argument, NULL, 'p'},
		{"help", no_argument, NULL, 'h'},
		{NULL, 0, NULL, 0}
	};
	while((opt = getopt_long(argc, argv, "i:p:h", lopts, NULL)) != -1)
	{
		switch(opt)
		{
			case 'i':
				SERVER_IP = optarg;
				break;
			case 'p':
				SERVER_PORT = atoi(optarg);
				break;
			case 'h':
				print_usage(argv[0]);
				return 0;		
		}
  	}

if(!SERVER_IP || !SERVER_PORT)
{
	print_usage(argv[0]);
	return 0;
}
//	if(argc < 3)
//	{
//		printf("Program usage: %s [ServerIP] [Port]\n",argv[0]);
//		return -1;
//	}
//	SERVER_IP = argv[1];
//	SERVER_PORT = atoi(argv[2]);
	
	conn_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(conn_fd < 0)
   	{
        	printf("create socket failure: %s\n", strerror(errno));
		return -1;
   	}
  		printf("socket create fd[%d]\n", conn_fd);
   
    		memset(&serv_addr, 0, sizeof(serv_addr));
    		serv_addr.sin_family = AF_INET;        
    		serv_addr.sin_port = htons(SERVER_PORT);
    		//serv_addr.sin_addr.s_addr = htonl(SERVER_IP);     //IP地址为0.0.0.0的地址
   	 	inet_aton(SERVER_IP,&serv_addr.sin_addr);
    
   		 printf("socke SERVER_IP\n" );
    
    	if(connect(conn_fd,(struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    	{
      		printf("connect to server [%s:%d] failure: %s\n", SERVER_IP, SERVER_PORT, strerror(errno));
    		return 0;
   	}
		printf("Connect sockserv[%s:%d] sucessfully!\n",SERVER_IP,SERVER_PORT);    
	while(1)
	{	
		if( write(conn_fd, MSG_STR, strlen(MSG_STR)) < 0 )
		{
           		printf("Write  data  to server[%s:%d] failure: %s\n", SERVER_IP, SERVER_PORT, strerror(errno));
	   		break;
		}
    		memset(buf, 0, sizeof(buf));
   	 	rv = read(conn_fd, buf, sizeof(buf));
    		if(rv < 0 )
    		{
      			printf("Read data from server failure: %s\n", strerror(errno));
      			break;

    		}
    		else if(rv == 0)
    		{	
      			printf("Client connect to server get disconnected\n");
      			break;
    		} 
    
   		 	printf("Read %d byte data from server: '%s'\n",rv, buf);
 
	}     
    	close(conn_fd);
}


