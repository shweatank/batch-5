#include<stdio.h>
#include<sys/msg.h>
#include<stdlib.h>
#include<stdio_ext.h>

struct msgq
{
	long msgtype;
	char str[20];
};

int main(int argc,char **argv)
{
	struct msgq msg;
	if(argc!=2)
	{
		printf("invalid args\n");
		return 0;
	}
	int msgid=msgget(1,IPC_CREAT|0666);
	if(msgid<0)
	{
		perror("msgget");
		return 0;
	}	
	msg.msgtype=atoi(argv[1]);
	while(1)
	{
		msgrcv(msgid,&msg,sizeof(msg.str),atoi(argv[1]),0);
		printf("received data : %s\n",msg.str);
	}
}
