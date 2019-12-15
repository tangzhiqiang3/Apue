#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <unistd.h>
#include<libgen.h>
#include<stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#define  FIFO_FILE1  ".fifo_chat1"
#define  FIFO_FILE2  ".fifo_chat2"

int g_stop = 0;

void sig_pipe(int signum)
{
	if(SIGPIPE==signum)
	{
		printf("get pipe broken signal and let program exit\n");
		g_stop = 1;
	}
}

int main(int argc, char **argv)
{
	int 	fdr_pipe;
	int 	fdw_pipe;
	int 	mode = 0;
	char	buf[1024];
	fd_set 	rdset;
	int	rv;

	if(argc != 2)
	{
		printf("Usage: %s [0/1]\n",basename(argv[0]));
		printf("This chat program need run twice, 1st time run with [0] and 2nd time with [1]\n");
		return -1;
	}
  	mode = atoi(argv[1]);
  /* 管道是一种半双工的通信方式，如果要实现两个进程间的双向通信则需要两个管道，即两个管道分别作为两个进程的读端和写端 */
	if(access(FIFO_FILE1, F_OK))
	{
		printf("FIFO file \"%s\" not exist and create it now\n", FIFO_FILE1);
		mkfifo(FIFO_FILE1, 0666);
	}
	if(access(FIFO_FILE2, F_OK))
        {
                printf("FIFO file: \"%s\" not exist and create it now\n", FIFO_FILE2);
                mkfifo(FIFO_FILE2, 0666);
        }
	
	signal(SIGPIPE, sig_pipe);
	
	if(0==mode)	//模式0
	{
		/* 只读方式打开命名管道FIFO_FILE1的读端；如果命名管道的写端被不打开，则open()将一直阻塞
		所已另外一个进程首先必须以写模式打开该文件FIFO_FILE1，否则会出现死锁 */
		printf("start open '%s' for read and it will blocked untill write endpoint opened...\n", FIFO_FILE1);
		if((fdr_pipe=open(FIFO_FILE1, O_RDONLY)) < 0 )
		{
			printf("Open filo[%s] for chat read endpiont failure: %s\n", FIFO_FILE1, strerror(errno));
			return -1;
		}
		printf("start open '%s' for write...\n", FIFO_FILE2);
		if((fdw_pipe=open(FIFO_FILE2, O_WRONLY)) < 0)
		{
			printf("Open file[%s] for chat write endpoint failure: %s\n", FIFO_FILE2, strerror(errno));
			return -1;
		}
	}
	else 
	{
		printf("start open '%s' for write and it will blocked untill read endpoint opened...\n", FIFO_FILE1);
		if((fdw_pipe=open(FIFO_FILE1, O_WRONLY)) < 0 )
                {       
                        printf("Open filo[%s] for chat read endpiont failure: %s\n", FIFO_FILE1, strerror(errno));
                        return -1;
                }
                printf("start open '%s' for write...\n", FIFO_FILE2);
                if((fdr_pipe=open(FIFO_FILE2, O_RDONLY)) < 0)
                {
                        printf("Open file[%s] for chat write endpoint failure: %s\n", FIFO_FILE2, strerror(errno));
                        return -1;
                }
	}
	
	printf("start chating with another program now, please input message now: \n");
	while(!g_stop)
	{
		FD_ZERO(&rdset);
		FD_SET(fdr_pipe, &rdset);
		FD_SET(STDIN_FILENO, &rdset);
		rv=select(fdr_pipe+1, &rdset, NULL, NULL, NULL);
		if(rv <= 0)
		{
			printf("select() get timeout or error: %s\n",strerror(errno));
			continue;
		}
		if(FD_ISSET(fdr_pipe, &rdset))
		{
			memset(buf, 0, sizeof(buf));
			rv=read(fdr_pipe, buf, sizeof(buf));
			if(rv < 0)
			{
				printf("read data from FIFO get error: %s\n", strerror(errno));
				break;
			}
			else if(0==rv)
			{
				printf("Another side of FIFO get closed and program will exit now\n");
				break;
			}
			printf("<-- %s",buf);
		}
		
		/* 如果从标准输入上有数据到来，则从标准输入上读数据后，将数据写入到作为标准输出的命名管道上给另外一个进程*/
		if(FD_ISSET(STDIN_FILENO, &rdset))
		{
			memset(buf, 0, sizeof(buf));
			fgets(buf, sizeof(buf), stdin);
			write(fdw_pipe, buf, strlen(buf));
		}	
	}
}

