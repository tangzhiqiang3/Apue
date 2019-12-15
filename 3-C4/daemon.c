#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<syslog.h>
#include<libgen.h>	/* basename */

int main(int argc, char **argv)
{
	char	*progname = basename(argv[0]);
	if(daemon(0, 0) < 0)
	{
		printf("Program daemon() failure: %s\n",strerror(errno));
		return -1;
	}
	openlog("daemon", LOG_CONS|LOG_PID, 0);
	syslog(LOG_NOTICE, "Program '%s' start running\n", progname);
	syslog(LOG_WARNING, "Program '%s' running with a warnning message\n", progname);
	syslog(LOG_EMERG, "Program '%s' running with a emergency message\n\n", progname);
	
	while(1)
	{
		//DO Something here
		;
	}
	
	syslog(LOG_NOTICE, "Program '%s' stop runnig\n", progname);
	closelog();
	
	return 0;
}

