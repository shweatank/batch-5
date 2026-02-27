#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>
#include<pthread.h>
pthread_t tid1,tid2;
sem_t sem1,sem2;
void* thread1_exe(void *a)
{
	for(int i=0;i<26;i++)
	{
		sem_wait(&sem1);
		printf("%c ",65+i);
		fflush(stdout);
		sem_post(&sem2);
	}
}
void *thread2_exe(void *a)
{
	for(int i=0;i<26;i++)
	{
		sem_wait(&sem2);
		printf("%c ",97+i);
		fflush(stdout);
		sem_post(&sem1);
	}
}
int main()
{
	sem_init(&sem1,0,1);
	sem_init(&sem2,0,0);
	pthread_create(&tid1,NULL,thread1_exe,NULL);
	pthread_create(&tid2,NULL,thread2_exe,NULL);
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	pthread_exit(NULL);
}
//sem1,sem2 are 2 binary semaphores...
//here common resouce is stdout buffer..thread1, thread2 are using it alternatively.
