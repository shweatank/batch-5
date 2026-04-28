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

	if(argc!=3)
	{
		printf("a.out sndmsg_type msg\n");
		return 1;
	}
	struct msg v;
	v.mtype=atoi(argv[1]);
	strcpy(v.data,argv[2]);
	int id=msgget(1,IPC_CREAT|0660);
	if(id==-1)
	{
		perror("msgget");
		return 1;
	}
	if(msgsnd(id,&v,strlen(v.data)+1,0)==-1)
	{
		perror("msgsnd");
		return 1;
	}
	printf("msg is send successfully\n");
}
