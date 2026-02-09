#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>
sem_t t1,t2;

void* uppercase(void*)
{
	char ch='A';
	for(int i=0;i<26;i++)
	{
	sem_wait(&t1);
		printf("%c",ch++);
		fflush(stdout);
//		sleep(1);
	sem_post(&t2);
	}
}

void* lowercase(void*)
{
	char ch='a';
	for(int i=0;i<26;i++)
	{
	sem_wait(&t2);
		printf("%c",ch++);
		fflush(stdout);
//		sleep(1);
	sem_post(&t1);
	}
}

int main()
{
	pthread_t td1,td2;

	sem_init(&t1,0,1);
	sem_init(&t2,0,0);

	pthread_create(&td1,NULL,uppercase,NULL);
	pthread_create(&td2,NULL,lowercase,NULL);

	pthread_join(td1,NULL);
	pthread_join(td2,NULL);

	sem_destroy(&t1);
	sem_destroy(&t2);
}
