#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
int flag=0,var=10;
pthread_t t1,t2;
void *f1(void *p)
{
	printf("in thread 1\n");
	while(flag!=0);
	flag=1;
	printf("in thread 1 values is %d\n",var);
	var++;
	sleep(15);
	var++;
	printf("in thread 1 values is %d\n",var);
	flag=0;
	printf("exiting from thread1\n");
	pthread_exit(NULL);
}

void *f2(void *p)
{
	printf("in thread 2 \n");
	while(flag!=0);
	flag=1;
	printf("in thread2 is value is %d\n",var);
        var++;
//sleep(15);
var++;
printf("in thread2 value is %d\n",var);
flag=1;
printf("exiting from thread2\n");
pthread_exit(NULL);

}

int main()
{
	pthread_create(&t1,NULL,f1,NULL);
	pthread_create(&t2,NULL,f2,NULL);

	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	pthread_exit(NULL);
}
