#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>

#define FTOK_PATH	"/dev/zero"
#define FTOK_PROJID	0x22

union semun
{
	int 	val;
	struct semid_ds * buf;
	unsigned short  * array;
	struct seminfo	* __buf;
};

int	semaphore_init(void);
int	semaphore_p(int semid);
int     semaphore_v(int semid);
void	semaphore_term(int semid);

int main(int argc, char **argv)
{
	int	semid;
	pid_t	pid;
	int	i;
	
	if((semid=semaphore_init()) < 0)
	{
		printf("semaphore initial failure: %s\n", strerror(errno));
		return -1;
	}
	if((pid=fork()) < 0)
	{
		printf("fork() failure: %s\n", strerror(errno));
		return -2;
	}
	else if(pid==0)
	{
		printf("Child process start running and do something now...\n");
		sleep(3);
		printf("Child process do something over...\n");
		semaphore_v(semid);

		sleep(1);
		printf("Child process exit now\n");
		exit(0);
	}
	printf("Parent process P operator wait child process over\n");
	semaphore_p(semid);

	printf("Parent process start destory semaphore and exit\n");
	sleep(2);

	printf("Child process exit\n");
	
	semaphore_term(semid);
	return 0;
}


int semaphore_init(void)
{
	key_t	key;
	int 	semid;
	union semun  sem_union;
	
	if((key=ftok(FTOK_PATH, FTOK_PROJID)) < 0)
	{
		printf("ftok() failure: %s\n", strerror(errno));
		return -1;
	}
	semid=semget(key, 1, IPC_CREAT|0644);
	if(semid < 0 )
	{
		printf("semget() get semid failure: %s\n", strerror(errno));
		return -2;
	}
	sem_union.val = 0;
	if(semctl(semid, 0, SETVAL, sem_union) < 0)
	{
		printf("semctl() set initial value failure: %s\n", strerror(errno));
		return -3;
	}
	printf("Semaphore get key_t[0x%x] and semid[%d]\n", key, semid);
	return semid;
}


void semaphore_term(int semid)
{
	union semun  sem_union;
	if(semctl(semid, 0, IPC_RMID, sem_union) < 0)
	{
		printf("semctl() delete semaphore ID failure: %s\n", strerror(errno));
		return;
	}
}


int semaphore_p(int semid)
{
	struct sembuf  _sembuf;
	
	_sembuf.sem_num = 0;
	_sembuf.sem_op = -1;
	_sembuf.sem_flg = SEM_UNDO;

	if(semop(semid, &_sembuf, 1) < 0)
	{
		printf("semop P operator failure: %s\n", strerror(errno));
		return -1;
	}
	return 0;
}


int semaphore_v(int semid)
{
         struct sembuf  _sembuf;
         
         _sembuf.sem_num = 0;
         _sembuf.sem_op = 1;
         _sembuf.sem_flg = SEM_UNDO;
 
         if(semop(semid, &_sembuf, 1) < 0)
         {
                 printf("semop V operator failure: %s\n", strerror(errno));
                 return -1;
         }
         return 0;
}

