#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
pthread_t tid1,tid2;
int var=7,flag=0;
void* fun1(void *a)
{
	printf("in thread1...\n");
	printf("var=%d\n",var);
	printf("entering critical section:\n");
	while(flag!=0);
	flag=1;
	++var;
	printf("var=%d\n",var);
	printf("var square=%d\n",var*var);
	++var;
	printf("critical section ended\n");
	printf("thread1 exiting...\n");
	flag=0;
	pthread_exit(NULL);
}
void* fun2(void *a)
{
	printf("in thread2...\n");
	printf("var=%d\n",var);
	printf("entering critical section:\n");
	while(flag!=0);
	flag=1;
	++var;
	printf("var=%d\n",var);
	printf("var square=%d\n",var*var);
	printf("critical section ended\n");
	printf("thread2 exiting...\n");
	flag=0;
	pthread_exit(NULL);
}
int main()
{
	pthread_create(&tid1,NULL,fun1,NULL);
	pthread_create(&tid2,NULL,fun2,NULL);
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	pthread_exit(NULL);
}
