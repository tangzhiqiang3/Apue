#include<stdio.h>
#include<stdlib.h>
#include<string.h>	
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>

#define FTOK_PATH	"/dev/zero"
#define FTOK_PROJID	0x22

typedef	struct s_msgbuf
{
	long	mtype;
	char	mtext[512];
} t_msgbuf;

int main(int argc, char **argv)
{
	int	i;
	int	msgid;
	int	msgtype;
	key_t	key;
	t_msgbuf msgbuf;
	
	if((key=ftok(FTOK_PATH, FTOK_PROJID)) < 0)
        {
                printf("ftok() get IPC taken failure: %s\n", strerror(errno));
                return -1;
        }
        msgid=msgget(key, IPC_CREAT|0666);
        if(msgid <0)
        {
                printf("msgget() create IPC failure: %s\n", strerror(errno));
                return -2;
        }
        msgtype=(int)key;
	printf("key[%d] msgid[%d] msgtype[%d]\n", (int)key, msgid, msgtype);
	for(i=0;i<4;i++)
	{	
		memset(&msgbuf, 0, sizeof(msgbuf));
		//msgbuf.mtype=msgtype;

		if(msgrcv(msgid, &msgbuf, sizeof(msgbuf.mtext), msgtype, IPC_NOWAIT)<0)
		{
			printf("msgrcv() receive message failure: %s\n", strerror(errno));
			break;
		}
		printf("receive message: %s\n", msgbuf.mtext);	
		sleep(1);
	}
	msgctl(msgid, IPC_RMID, NULL);
	return 0;
}


