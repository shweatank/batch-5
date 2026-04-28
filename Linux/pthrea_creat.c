#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
pthread_t t1,t2;
int var=10;
void *f1(void *p)
{
	printf("in f1 thread var=%d\n",var);
	var++;
//	sleep(30);
	printf("f1 exiting\n");
	pthread_exit(NULL);
}
void *f2(void *p)
{
	printf("in f2 var2=%d\n",var);
//	sleep(30);
	printf("f2 exiting\n");
	pthread_exit(NULL);
}

int main()
{
	printf("in main.. %d\n",var);
	pthread_create(&t1,NULL,f1,NULL);
	pthread_create(&t2,NULL,f2,NULL);

	printf("two jobs are created..\n");
        sleep(30);
	printf("Main exiting\n");
	pthread_exit(NULL);
	return 0;

}
