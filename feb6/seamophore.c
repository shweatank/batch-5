#include<stdio.h>
#include<semaphore.h>
#include<sys/types.h>
#include<pthread.h>
#include<unistd.h>


char ch='A';
sem_t lock1,lock2;

void* lower(void *arg)
{
	int i=0;
	while(i<26)
	{
		sem_wait(&lock1);
		printf("%c",ch+i);
		fflush(stdout);
		sem_post(&lock2);
		i++;
		sleep(1);
	}
}

void* upper(void *arg)
{
	int i=0;
	while(i<26)
	{
		sem_wait(&lock2);
		printf("%c",ch+32+i);
		fflush(stdout);
		sem_post(&lock1);
		i++;
//		sleep(1);
	}
}

int main()
{
	pthread_t td1,td2;
	sem_init(&lock1,0,1);
	sem_init(&lock2,0,0);

	pthread_create(&td1,NULL,upper,NULL);
	pthread_create(&td2,NULL,lower,NULL);

	pthread_join(td1,NULL);
	pthread_join(td2,NULL);

	sem_destroy(&lock1);
	sem_destroy(&lock2);
}
