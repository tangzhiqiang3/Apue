#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
 
int main(int argc, char *argv[])
{
	struct hostent *p;
	int i;
 
	if (argc < 2)
	{	printf("socket DNS %s failure:%s\n",argv[0],strerror(errno));
		return -1;
 	}
	p = gethostbyname(argv[1]);
	printf("hostname %s\n", p->h_name);
	printf("address:");
	for (i = 0; p->h_addr_list[i]; i++)
	{
		printf("%s\n", inet_ntoa(*(struct in_addr *)p->h_addr_list[i]));
	}
	printf("\n");
 
	return 0;
}

