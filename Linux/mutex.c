#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

pthread_t t1,t2;
pthread_mutex_t a;
int var=10,flag=0;
void *f1(void *p)
{
	printf("in thread 1\n");
	pthread_mutex_lock(&a);
	printf("in thread 1 var is %d\n",var);
	var++;
	sleep(15);
	var++;
	printf("in thread 2 var is %d\n",var);
	pthread_mutex_unlock(&a);
	printf("exiting from thread 1\n");
	pthread_exit(NULL);
}

void *f2(void *p)
{
	printf("in thread 2\n");
	pthread_mutex_lock(&a);
	printf("in thread 2 var is %d\n",var);
	var++;
	sleep(15);
	var++;
	printf("in thread 2 var is %d\n",var);
	pthread_mutex_unlock(&a);
	printf("exiting from thread 2\n");

}

int main()
{
	pthread_create(&t1,NULL,f1,NULL);
	pthread_create(&t2,NULL,f2,NULL);

	pthread_join(t1,NULL);
	pthread_join(t2,NULL);

	pthread_exit(NULL);
}
