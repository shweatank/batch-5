#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/msg.h>
#include<sys/ipc.h>
#include<stdlib.h>
pthread_t ad,s,m,d;
int res,a[2];
pthread_mutex_t m1,m2;
int turn=1;
void *add(void *ar)
{
	while(turn!=1);
	pthread_mutex_lock(&m1);
	res=a[0]+a[1];
	pthread_mutex_unlock(&m1);
	pthread_exit(0);
		
}
void *sub(void *ar)
{
	while(turn!=3);
	pthread_mutex_lock(&m1);
        res=a[0]-a[1];
	pthread_mutex_unlock(&m1);
	pthread_exit(0);
}

int main(int argc,char *argv[])
{
	if(argc<3){printf("a.out a,b\n");return 0;}
	a[0]=atoi(argv[1]);
	a[1]=atoi(argv[2]);
	pthread_mutex_init(&m1,NULL);
	//pthread_mutex_init(&m2,NULL);
	pthread_create(&ad,0,add,0);
	pthread_create(&s,0,sub,0);
	pthread_join(ad,0);
	pthread_join(s,0);
	printf("add;%d\n",res);
	while(turn!=4);
//	pthread_join(s,0);
	printf("sub:%d\n",res);
	pthread_mutex_unlock(&m2);
	pthread_exit(0);
}
