#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>
#include<execinfo.h>

int 	g_sigstop = 0;
void signal_stop(int signum)
{
	if(SIGTERM == signum)
	{
		printf("SIGTERM signal detected\n");
	}
	else if(SIGALRM == signum)
	{
 		printf("SIGALRM signal detected\n");
		g_sigstop = 1;
	}
}

void signal_user(int signum)
{
 	if(SIGUSR1 == signum)
        {
              	printf("SIGUSR1 signal detected\n");
      	}
        else if(SIGUSR2 == signum)
        {
            	printf("SIGUSR2 signal detected\n");
        }
	g_sigstop = 1;
}

void signal_code(int signum)
{
     	if(SIGBUS == signum)
     	{
             	printf("SIGBUS signal detected\n");
        }
        else if(SIGILL == signum)
        {
             	printf("SIGILL signal detected\n");
        }       
    	else if(SIGSEGV == 0)
	{
		printf("SIGSEGV signal detected\n");
	}
	
 	exit(-1);
}
int main(int argc, char **argv)
{
	char			*ptr=NULL;
	struct sigaction	sigact, sigign;
	
	/* Method1: Use signal() install signal */	
	signal(SIGTERM, signal_stop);
	signal(SIGALRM, signal_stop);
	
 	signal(SIGBUS, signal_code);
	signal(SIGILL, signal_code);
	signal(SIGSEGV, signal_code);

	/* Method2: Use sigcationl() install signal */
	
	/* Initialize the catch signal structure */
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = 0;
	sigact.sa_handler = signal_user;
	
	/* Setup the ignore signal */
	sigemptyset(&sigact.sa_mask);
	sigign.sa_flags = 0;
	sigign.sa_handler = SIG_IGN;		//SIG_DEL 系统默认动作

	sigaction(SIGINT, &sigign, 0);		//ignore SIGINT signal by CTRL+C
	sigaction(SIGUSR1, &sigact, 0);		//catch SIGUSR1
	sigaction(SIGUSR2, &sigact, 0);		//catch SIGUSR2

	printf("Program start running for 20 seconds...\n");
	alarm(20);

	while(!g_sigstop)
	{
		;
	}
	
	printf("Program start stop running...\n");
	
	printf("Invalid pointer operator will raise SIGSEGV signal\n");
	*ptr = 'h';

	return 0;
}


