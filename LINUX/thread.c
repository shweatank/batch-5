#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
pthread_t tid1;
void *fun1(void *x)
{
	printf("hello\n");
	pthread_exit(0);
}
int main()
{
	pthread_create(&tid1,0,fun1,0);
	pthread_join(tid1,0);
	pthread_exit(0);
}
