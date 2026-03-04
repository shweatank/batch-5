#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#define SIZE 5

int buff[SIZE],in=0,out=0;

pthread_t t1,t2;
sem_t full,empty,mutex;

void *producer(void *args)
{
	int i;
	for(i=1;i<=10;i++)
	{
		sem_wait(&empty);
		sem_wait(&mutex);
		buff[in]=i;
		in =(in+1)%SIZE;
		printf("produced is %d\n",i);
		sem_post(&mutex);
		sem_post(&full);

	}
	return NULL;
}

void *consumer(void *args)
{
	int i;
	for(i=1;i<=10;i++)
	{
		sem_wait(&full);
		sem_wait(&mutex);
		i=buff[out];
		printf("consumed is %d\n",i);
		out=(out+1)%SIZE;
		sem_post(&mutex);
		sem_post(&empty);
	}
	return NULL;
}

int main()
{
	sem_init(&full,0,0);
	sem_init(&empty,0,SIZE);
	sem_init(&mutex,0,1);
	pthread_create(&t1,NULL,producer,NULL);
	pthread_create(&t2,NULL,consumer,NULL);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	sem_destroy(&full);
	sem_destroy(&empty);
	sem_destroy(&mutex);
}
