#include<stdio.h>
#include<sys/msg.h>
struct msgq
{
	long mtype;
	int res;
};

int main()
{
	struct msgq msg;
	int msgid=msgget(1,IPC_CREAT|0666);
	perror("msgget");
	for(int i=0;i<4;i++)
	{
		msgrcv(msgid,&msg,sizeof(msg.res),1,0);
		printf("result : %d\n",msg.res);
	}
	msgctl(msgid,IPC_RMID,NULL);
}
