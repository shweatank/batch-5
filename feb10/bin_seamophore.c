#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>

sem_t lock1,lock2;
int shared;

void* increment(void *)
{
	for(int i=0;i<5;i++)
	{
		sem_wait(&lock1);		//initially 1 so sem wait decrements lock1 and run now 0
		printf("increment : %d\n",++shared);
		sem_post(&lock2);		//initially 0 its incremented 
		usleep(1);
	}
	return NULL;
}

void* decrement(void *)
{
	for(int i=0;i<5;i++)
	{
		sem_wait(&lock2);
		printf("decrement : %d\n",--shared);
		sem_post(&lock1);
		usleep(1);
	}
	return NULL;
}

int main()
{
	pthread_t td1,td2;

	sem_init(&lock1,0,1);
	sem_init(&lock2,0,0);

	pthread_create(&td1,NULL,increment,NULL);
	pthread_create(&td2,NULL,decrement,NULL);

	pthread_join(td1,NULL);
	pthread_join(td2,NULL);

	sem_destroy(&lock1);
	sem_destroy(&lock2);

	return 0;
}
