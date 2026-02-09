#include<stdio.h>
#include<sys/msg.h>
#include<string.h>
#include<stdio_ext.h>
#include<stdlib.h>

struct msgq
{
	long msgtype;
	char str[20];
};

int main(int argc,char **argv)
{
	char str[20];
	if(argc!=2)
	{
		printf("invalid args\n");
		return 0;
	}
	int key=1;
	int msgid=msgget(key,IPC_CREAT|0666);
	if(msgid<0)
	{
		perror("msgget");
		return 0;
	}
	struct msgq msg;
	msg.msgtype=atoi(argv[1]);
	while(1)
	{
		__fpurge(stdin);
		scanf("%s",str);
		strcpy(msg.str,str);
		msgsnd(msgid,&msg,sizeof(msg.str),0);
		printf("msg sent : %s\n",str);
	}
	return 0;
}
