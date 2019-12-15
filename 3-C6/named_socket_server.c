#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>

#define SOCKET_PATH   "/tmp/socket.domain"


int main(int argc, char **argv)
{
	int 			rv = -1;
	int 			listen_fd,client_fd = -1;
	struct sockaddr_un	serv_addr;
	struct sockaddr_un	cli_addr;
	socklen_t		cliaddr_len;
	char			buf[1024];

	if((listen_fd=socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
	{
		printf("Create socket failure: %s\n",strerror(errno));
		return -1;
	}
	
	printf("Create socket fd[%d] successfully\n", listen_fd);
	
	if(!access(SOCKET_PATH, F_OK))
	{
		unlink(SOCKET_PATH);
	}
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sun_family = AF_UNIX;
	strncpy(serv_addr.sun_path, SOCKET_PATH, sizeof(serv_addr.sun_path)-1 );
	
	if(bind(listen_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("socket [%d] bind address fialure: %s\n", listen_fd, strerror(errno));
		unlink(SOCKET_PATH);
		return -2;
	}
	printf("Socket %d bind on \"%s\" OK\n", listen_fd, SOCKET_PATH);
	if(listen(listen_fd, 13))
	{
		printf("listen socket failure: %s\n", strerror(errno));
		return -3;
	}
	printf("listen socket ok\n");
	while(1)
	{	
		printf("\nStart waiting and accept new client connect...\n");
		if((client_fd=accept(listen_fd, (struct sockaddr*)&cli_addr, &cliaddr_len)) < 0)
		{
			printf("accept socket failure: %s\n", strerror(errno));
			return -4;
		}
		printf("accept [%d] socket ok, start read/write client data\n", listen_fd);

		memset(buf, 0, sizeof(buf));
		if((rv=read(client_fd, buf, sizeof(buf))) < 0)
		{
			printf("read data from client socket [%d] failure: %s\n", client_fd, strerror(errno));
			close(client_fd);
			continue;
		}
		else if(rv==0)
		{
			printf("Client socket [%d] disconnected\n", client_fd);
			close(client_fd);
			continue;
		}
		printf("Read %d byte data from client [%d]\n and echo it back: %s\n",rv, client_fd, buf);
		
		if(write(client_fd, buf, rv) < 0)
		{
			printf("socket [%d] write back client failure: %s", client_fd, strerror(errno));
			close(client_fd);
		}
		
		sleep(1);
		close(client_fd);
	}
	close(listen_fd);
}	

