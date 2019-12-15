#include<stdio.h>
#include<string.h>
#include<errno.h>
#include <sys/time.h>
#include <sys/resource.h>

void	print_limit(char *name, int resoure)
{
	struct rlimit limit;
	if(getrlimit(resoure, &limit) < 0)
	{
		printf("getrlimit for  failure: %s\n", strerror(errno));	
		return ;
	}
	printf("%-15s ",name);
	if(limit.rlim_cur == RLIM_INFINITY)
	{
		printf("(infinite)    ");
	}
	else
	{
		printf("%-15ld", limit.rlim_cur);
	}

	if(limit.rlim_max == RLIM_INFINITY)
        {
      	      printf("(infinite)    ");
        }
        else
        {
       	      printf("%-15ld", limit.rlim_max);
        }
	printf("\n");
}

int main(void)
{
	struct rlimit limit={0};
	print_limit("RLIMIT_NPROC", RLIMIT_NPROC);		//用户课拥有的最大进程数
	print_limit("RLIMIT_DATA", RLIMIT_DATA);		//进程数据段的最大值
	print_limit("RLIMIT_STACK", RLIMIT_STACK);		//最大的进程堆栈，以字节为单位
	print_limit("RLIMIT_NOFILE", RLIMIT_NOFILE);		//指定比进程可打开的最大文件描述词大一的值，超出此值，将会产生EMFILE错误
	printf("\nAfter set RLIMIT_NOFILE:\n");
	getrlimit(RLIMIT_NOFILE, &limit);
	limit.rlim_cur = limit.rlim_max;
	setrlimit(RLIMIT_NOFILE, &limit);
	
	print_limit("RLIMIT_NOFILE", RLIMIT_NOFILE);

	return 0;
}

