#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/wait.h>

#define MSG_STR  "This message is from parent: Hello, child process!"

int main(int argc, char **argv)
{
	int 	rv;
	int	pip_fd[2];
	int 	pid;
	char 	buf[1024];
	int	wstatus;	
	int 	tv;	

	if(pipe(pip_fd) < 0)
	{
		printf("Create pipe failure: %s\n",strerror(errno));
		return -1;
	}
	
	if((pid=fork()) < 0)
	{
		printf("fork() create failure: %s\n",strerror(errno));
		return -2;
	}
	else if(pid==0)
	{
		/* turn off child process write port */
		close(pip_fd[1]);
		memset(buf, 0, sizeof(buf));
		rv=read(pip_fd[0], buf, sizeof(buf));
		if(rv < 0)
		{
			printf("child process read failure: %s\n",strerror(errno));
			return -3;
		}
		printf("Child process read %d byte data from pipe: /%s/\n",rv, buf);
		return 0;
	}
	
	/* parent process start running and wait child process exit */
	printf("Parent process start running and close its read port\n");
	close(pip_fd[0]);	

	//memset(buf, 0, sizeof(buf));
	if((tv=write(pip_fd[1], MSG_STR, strlen(MSG_STR))) < 0)
	{
		printf("Parent process write failure: %s\n",strerror(errno));
		return -1;
	}
	printf("Parent process write %d byte data: /%s/\n", tv, MSG_STR);
	printf("Parent start wait child process exit...\n");
	wait(&wstatus);
	
	return 0;
}

