#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>

int main(int argc, char **argv)
{
	pid_t	pid;
	printf("Parent process PID[%d] start running...\n",getpid() );
	
	pid = fork();
	if(pid < 0)
	{
		printf("fork() create child process failure: %s\n",strerror(errno));
		return -1;
	}
	else if(pid == 0)
	{
		printf("Child process PID[%d] start running,my parent PID is [%d]\n", getpid(), getppid() );
		return 0;
	}
	else 
	{
		printf("Parent process PID[%d] start running, and child process PID is [%d]\n", getpid(), pid);
		sleep(1);
		return 0;
	}
}

