#include<stdio.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<pthread.h>
struct msgbuf
{
	long msgtyp;
	int res;
};
int a,b;
pthread_mutex_t m1;
pthread_t t1,t2,t3,t4;
void *add_thread(void * n)
{
	struct msgbuf m;
	m.msgtyp=1;
	while(1)
	{
		pthread_mutex_lock(&m1);
	m.res=a+b;
	int id=msgget(1,IPC_CREAT|0666);
	msgsnd(id,&m,sizeof(m.res),0);
	pthread_mutex_unlock(&m1);
	}

}
void *sub_thread(void * n)
{
	struct msgbuf m;
	m.msgtyp=2;
	while(1)
	{
	m.res=a-b;
	int id=msgget(1,IPC_CREAT|0666);
	msgsnd(id,&m,sizeof(m.res),0);
	}
}

void *mul_thread(void * n)
{
	struct msgbuf m;
	m.msgtyp=3;
	while(1)
	{
	m.res=a*b;
	int id=msgget(1,IPC_CREAT|0666);
	msgsnd(id,&m,sizeof(m.res),0);
	}
}

void *div_thread(void * n)
{
	struct msgbuf m;
	m.msgtyp=4;
	while(1)
	{
	m.res=a/b;
	int id=msgget(1,IPC_CREAT|0666);
	msgsnd(id,&m,sizeof(m.res),0);
	}
}
int main()
{
	printf("enter the a and b\n");
	scanf("%d %d",&a,&b);
	pthread_create(&t1,NULL,add_thread,NULL);
	pthread_create(&t2,NULL,sub_thread,NULL);
	pthread_create(&t3,NULL,mul_thread,NULL);
	pthread_create(&t4,NULL,div_thread,NULL);


}
