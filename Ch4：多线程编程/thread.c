#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>

void *thread_worker1(void *args);
void *thread_worker2(void *args);

int main(int argc, char **argv)
{
	int		shared_var = 1000;
	pthread_t	tid;
	pthread_attr_t	thread_attr;
	
	if(pthread_attr_init(&thread_attr) < 0 )
	{
		printf("pthread_attr_init() failure: %s\n", strerror(errno));
		return -1;
	}
	if(pthread_attr_setstacksize(&thread_attr, 120*1024) < 0 )
	{
		printf("pthread_attr_setstacksize() failure: %s\n", strerror(errno));
 	       	return -1;
	}
	if(pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED) < 0 )
	{
		printf("pthread_attr_setdetachstate() failure: %s\n", strerror(errno));
		return -1;
	}
	
	//printf("Thread worker1 tid[%ld] create ok\n", tid);
	pthread_create(&tid, &thread_attr, thread_worker1, &shared_var);
	printf("Thread worker1 tid[%ld] create ok\n", tid);

	//printf("Thread worker2 tid[%ld] create ok\n", tid);
        pthread_create(&tid, NULL, thread_worker2, &shared_var);
	printf("Thread worker2 tid[%ld] create ok\n", tid);

	pthread_attr_destroy(&thread_attr);

	/* Wait until thread worker2 exit() */
	pthread_join(tid, NULL);

	while(1)
	{
		printf("Main/Control thread shared_var: %d\n", shared_var);
		sleep(10);
	}
}

	void *thread_worker1(void *args)
	{
		int 	*ptr = (int *)args;
		if(!args)
		{
			printf("%s() get invalid arguments\n", __FUNCTION__);
			pthread_exit(NULL);
		}
		printf("Thread worker1 [%ld] start running...\n",pthread_self());
		
		while(1)
		{
			printf("+++: %s before shared_var++: %d\n", __FUNCTION__, *ptr);
			*ptr += 1;
			sleep(2);
			
			printf("+++: %s after shared_var++: %d\n", __FUNCTION__, *ptr);
			return NULL;
		}
	}
 	void *thread_worker2(void *args)
      	{
          	int     *ptr = (int *)args;
               	if(!args)
               	{
                        printf("%s() get invalid arguments\n", __FUNCTION__);
                        pthread_exit(NULL);
                }
               	printf("Thread worker2 [%ld] start running...\n",pthread_self());
                 
                while(1)
                {
                        printf("---: %s before shared_var++: %d\n", __FUNCTION__, *ptr);
                        *ptr += 1;
                        sleep(2);
                       
                    	printf("---: %s after shared_var++: %d\n", __FUNCTION__, *ptr);
              	}         
			printf("Thread worker2 exit...\n");
			return NULL;
                
	}


