#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/msg.h>
#include<sys/ipc.h>
struct msgbuff
{
	long mtype;
	int data;
}v;
void isr(int num)
{
	printf("main received signal:%d\n",num);
}
int main(int argc,char*argv[])
{
	int id=msgget(7,IPC_CREAT|0664);
	if(msgget<0){perror("msgget");return 0;}
	if(argc!=3){printf("a.out a b\n");return 0;}
	int r1,r2,r3,r4,a=atoi(argv[1]),b=atoi(argv[2]);
	if((r1=fork())==-1){perror("fork");return 0;}
	if(r1>0)
	{
		if((r2=fork())==-1){perror("fork");return 0;}
		if(r2>0)
		{	
			if((r3=fork())==-1){perror("fork");return 0;}
			if(r3>0)
			{
				if((r4=fork())==-1){perror("fork");return 0;}
				if(r4>0)
				{
					signal(SIGUSR1,isr);
					pause();
					msgrcv(id,&v,sizeof(int),1,0);
					printf("add:%d\n",v.data);
				}
				else
				{
					printf("add process pid:%d\n",getpid());
					v.mtype=1;
					v.data=a+b;
					msgsnd(id,&v,sizeof(int),0);
					kill(getppid(),SIGUSR1);
				}
			}
			else
			{

			}

		}
		else
		{

		}
	}
	else
	{

	}
}
