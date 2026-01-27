#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/msg.h>
struct msgbuff
{
	long mtype;
	int data;
}v;
int main()
{
	int id=msgget(7,IPC_CREAT|0664);
	msgrcv(id,&v,sizeof(int),1,0);
	printf("addition:%d\n",v.data);
	msgrcv(id,&v,sizeof(int),2,0);
	printf("subtraction:%d\n",v.data);
	msgrcv(id,&v,sizeof(int),3,0);
	printf("multiplication:%d\n",v.data);
	msgrcv(id,&v,sizeof(int),4,0);
	printf("division:%d\n",v.data);
}

