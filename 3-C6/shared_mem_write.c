#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<stdlib.h>
#include<string.h>

#define FTOK_PATH	"/dev/zero"
#define FTOK_PROJID	0x22

typedef struct	st_student
{
	char 	name[64];
	int	age;
}t_student;



int main(int argc, char **argv)
{
	int	shmid;
	key_t	key;
	int	i;
	t_student *student;

	if((key=ftok(FTOK_PATH, FTOK_PROJID)) < 0)
	{
		printf("ftok() get IPC token failure: %s\n", strerror(errno));
		return -1;
	}
	shmid=shmget(key, sizeof(t_student), IPC_CREAT|0666);
	printf("key [0x%x]\n", key);
	printf("shmid [%d]\n", shmid);
	if(shmid < 0)
	{
		printf("shmget() creat IPC share memory failure: %s\n", strerror(errno));
		return -2;
	}
	
	student=shmat(shmid, NULL, 0);
	if((void *)-1==student)
	{
		printf("shmat() share memory failure: %s\n", strerror(errno));
		return -3;
	}
	strncpy(student->name, "zhangshan", sizeof(student->name));
	student->age=18;
	for(i=0;i<4;i++)
	{
		student->age++;
		printf("Student '%s' age [%d]\n", student->name, student->age);
		sleep(1);
	}
	
	shmdt(student);
	//shmctl(shmid, IPC_RMID, NULL);
	
	return 0;
}

