#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<pthread.h>
#include<sys/msg.h>

struct data
{
	int a;
	int b;
	int sum;
};

struct msgbuf
{
	long mtype;
	struct data d;
};

void main()
{
	struct msgbuf v;
	int id;
	id=msgget(1,IPC_CREAT|0664);
	if(id<0)
	{
		perror("msgget");
		return;
	}
	msgrcv(id,&v,sizeof(struct data),2,0);
	perror("msgrcv");

	printf("%d\n",v.d.sum);
}
