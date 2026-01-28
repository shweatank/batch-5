#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/wait.h>
struct msgbuf
{
	long mtype;
	char data[24];
};
void main()
{
	struct msgbuf msg;
	pid_t pid;
	pid=fork();
	if(pid==0)
	{
		int id;
		id=msgget(3,IPC_CREAT|0664);
		int ret;
		ret=msgrcv(id,&msg,sizeof(msg),1,0);
		printf("ret=%d\n",ret);
		int a,b,op;
		a=(int)msg.data[0];
		b=(int)msg.data[4];
		op=msg.data[8];

		switch(op)
		{
			case '+': printf("Addition %d and %d is %d\n",a,b,a+b); break;
			case '-': printf("Subtraction %d and %d is %d\n",a,b,a-b); break;
			case '*': printf("Multiplication %d and %d is %d\n",a,b,a*b); break;
			case '/': printf("Division %d and %d is %d\n",a,b,a/b); break;
		//	default: printf("Unknown option\n");
		}

		msgctl(id,IPC_RMID,NULL);

	}
	else
	{
		int id;
		id=msgget(3,IPC_CREAT|0664);
		int a,b;
		char op;
		printf("Enter a, b and op:\n");
		scanf("%d %d %c",&a,&b,&op);

		msg.data[0]=a;
		msg.data[4]=b;
		msg.data[8]=op;
		msg.data[3]='\0';

		msg.mtype=1;

		msgsnd(id,&msg,sizeof(msg),0);
		printf("Message queue: %d\n",id);

		wait(NULL);
	}
}
