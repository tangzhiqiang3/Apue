#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<ctype.h>

#define TMP_FILE  "/tmp/.ifconfig.log" 

int main(int argc, char **argv)
{
	int	fd;
	int 	rv;
	pid_t	pid;
	char	buf[1024];
	char 	*ip_start;
	char	*ip_end;
	char	*ptr;
	char	ipaddr[16];
	FILE	*fp;
	
	if((fd=open(TMP_FILE, O_RDWR|O_CREAT|O_TRUNC, 0644)) < 0)
	{
		printf("Readirect standard output to file failure: %s\n",strerror(errno));
		return -1;
	}
	pid=fork();
	if(pid < 0)
	{
		printf("fork() create child process failure: %s\n",strerror(errno));
		return -1;
	}
	else if(pid == 0)
	{
		printf("Child process excute ifconfig program\n");
		dup2(fd, STDOUT_FILENO);
		execl("/sbin/ifconfig", "ifconfig", "eth0", NULL);
		printf("Child process excute another program,will not return here.Return here means execl() error\n");
		return -1;
	}
	
	else //if(pid >0)
	{
		sleep(3);
		//printf("test1\n");
	}
	//printf("test2\n");
	
	memset(buf, 0, sizeof(buf));
	//printf("test3\n");
	rv=read(fd, buf, sizeof(buf));
	//if(rv < 0)
	//printf("Read file failure: %s\n",strerror(errno));
	printf("Read %d byte data directly read after child process write\n",rv);
	
	memset(buf, 0, sizeof(buf));
	lseek(fd, 0, SEEK_SET);
	rv=read(fd, buf, sizeof(buf));
	printf("Read %d byte data after lseek: %s\n",rv, buf);
	
	fp=fdopen(fd, "r");
	fseek(fp, 0, SEEK_SET);
	while(fgets(buf, sizeof(buf), fp))
	{
		if(strstr(buf, "地址:"))
		{
			ptr=strstr(buf, "地址:");	
			if(!ptr)
			{
				break;
			}
			ptr+=strlen("地址:");
			//while(isblank(*ptr))
			//ptr++;
			ip_start=ptr;
			while(isblank(*ptr))
			ptr++;
			ip_end=ptr;
			memset(ipaddr, 0, sizeof(ipaddr));
			memcpy(ipaddr, ip_start, ip_end-ip_start);
			break;
		}
	}
	printf("Parser and get IP address: %s\n",ipaddr);
	fclose(fp);
	unlink(TMP_FILE);
	return 0;
}

