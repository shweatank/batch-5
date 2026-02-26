#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/msg.h>

pthread_t p1,p2,p3,p4;
pthread_mutex_t pt;
int msgid;

struct msgq
{
	long msgtype;
	int res;
};
	

int n1=10,n2=15;

void send(int message)
{
	struct msgq msg;
	msg.msgtype=1;
	pthread_mutex_lock(&pt);
	msg.res=message;
	msgsnd(msgid,&msg,sizeof(msg.res),0);
	pthread_mutex_unlock(&pt);
}

void *sum(void *dummy)
{
	printf("sum : %d\n",n1+n2);	
	int r=n1+n2;
	send(r);
	return NULL;
}

void *sub(void *d)
{
	printf("sub : %d\n",n1-n2);
	int r=n1-n2;
	send(r);
	return NULL;
}

void *mul(void *d)
{
	printf("mul : %d\n",n1*n2);
	int r=n1*n2;
	send(r);
	return NULL;
}

void *divi(void *d)
{
	printf("div : %d\n",n1/n2);
	int r=n1/n2;
	send(r);
	return NULL;
}

int main()
{
	msgid=msgget(1,IPC_CREAT|0666);
	
	pthread_create(&p1,NULL,sum,NULL);
	pthread_create(&p2,NULL,sub,NULL);
	pthread_create(&p3,NULL,mul,NULL);
	pthread_create(&p4,NULL,divi,NULL);

	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	pthread_join(p3,NULL);
	pthread_join(p4,NULL);

	pthread_exit(NULL);
}
