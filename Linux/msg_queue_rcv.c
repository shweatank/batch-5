#include<stdio.h>
#include<sys/msg.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<string.h>

struct msg
{
	int mtype;
	char data[20];
};
int main(int argc,char *argv[])
{

	if(argc!=2)
	{
		printf("invalid\n");
		return 1;
	}
	struct msg v;
	int id;
	id=msgget(1,IPC_CREAT|0666);
	if(id==-1)
	{
		perror("msgget");
	}
	if(msgrcv(id,&v,sizeof(v.data),atoi(argv[1]),0)==-1)
	{
		perror("msgrcv");
		return 1;
	}
	printf("received data:%s and id:%d\n",v.data,v.mtype); 
}
