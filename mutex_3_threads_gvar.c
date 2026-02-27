#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/msg.h>
#include<sys/ipc.h>
#include<stdlib.h>
#include<semaphore.h>
pthread_t ad,s,m,d;
int res,a[2];
pthread_mutex_t m1,m2;
sem_t s1,s2;
void *add(void *ar)
{
	pthread_mutex_lock(&m1);
	res=a[0]+a[1];
	pthread_mutex_unlock(&m1);
	sem_post(&s1);
	pthread_exit(0);
		
}
void *sub(void *ar)
{
	pthread_mutex_lock(&m1);
        res=a[0]-a[1];
	pthread_mutex_unlock(&m1);
	sem_post(&s2);
	pthread_exit(0);
}

int main(int argc,char *argv[])
{
	if(argc<3){printf("a.out a,b\n");return 0;}
	a[0]=atoi(argv[1]);
	a[1]=atoi(argv[2]);
	pthread_mutex_init(&m1,NULL);
	sem_init(&s1,0,0);
	sem_init(&s2,0,0);
	pthread_create(&ad,0,add,0);
	pthread_create(&s,0,sub,0);
	sem_wait(&s1);
	pthread_mutex_lock(&m1);
	printf("add;%d\n",res);
	pthread_mutex_unlock(&m1);
//	pthread_join(s,0);
	sem_wait(&s2);
	pthread_mutex_lock(&m1);
	printf("sub:%d\n",res);
	pthread_mutex_unlock(&m1);
	pthread_join(ad,0);
	pthread_join(s,0);
	pthread_exit(0);
}
