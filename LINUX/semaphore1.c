#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>
#include<pthread.h>
int var=7;
pthread_t tid1,tid2;
sem_t sem1;
void* fun1(void *a)
{
	printf("in thread1...\n");
	printf("var=%d\n",var);
	printf("entering critical section:&aquiring semaphore\n");
	sem_wait(&sem1);
	++var;
	printf("var=%d\n",var);
	printf("var square=%d\n",var*var);
	++var;
	printf("critical section ended\n");
	printf("thread1 exiting&releasing semaphore...\n");
	sem_post(&sem1);
	pthread_exit(NULL);
}
void* fun2(void *a)
{
	printf("in thread2...\n");
	printf("var=%d\n",var);
	printf("entering critical section and aquiring semaphore:\n");
	sem_wait(&sem1);
	++var;
	printf("var=%d\n",var);
	printf("var square=%d\n",var*var);
	printf("critical section ended\n");
	printf("thread2 exiting and releasing sem1...\n");
	sem_post(&sem1);
	pthread_exit(NULL);
}
int main()
{
	sem_init(&sem1,0,1);
	pthread_create(&tid1,NULL,fun1,NULL);
	pthread_create(&tid2,NULL,fun2,NULL);
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	pthread_exit(NULL);
}
