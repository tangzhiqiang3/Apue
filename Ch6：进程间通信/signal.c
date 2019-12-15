#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include <signal.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>

int g_child_stop = 0;
int g_parent_run = 0;

void sig_child(int signum)
{
	if(SIGUSR1==signum)
	g_child_stop = 1;
}

void sig_parent(int signum)
{
        if(SIGUSR2==signum)
        g_parent_run = 1;
}

int main(int argc, char **argv)
{
	int 	pid;
	int	wstatus;

	signal(SIGUSR1, sig_child);
	signal(SIGUSR2, sig_parent);

	if((pid=fork()) < 0)
	{
		printf("fork() create child process failure: %s\n",strerror(errno));
		return -1;
	}

	else if(pid==0)
	{
		printf("Child process running and send parent signal\n");
		kill(getppid(), SIGUSR2);
		while(!g_child_stop)
		{
			sleep(1);
		}
		printf("Child process recevie signal from parent and exit now\n");
		return 0;
	}
	
	printf("Parent process hangs up untill receive signal from child\n");
	while(!g_parent_run)
	{
		sleep(1);
	}

	/* parent process can do something here, then tell child process to exit */
	printf("Parent process running now and send child a signal to exit\n");
	kill(pid, SIGUSR1);
	
	/*Parent wait child process exit*/
	wait(&wstatus);
	return 0;
}

