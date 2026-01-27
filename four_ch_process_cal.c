#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<stdio_ext.h>
struct msgbuff
{
	long mtype;
	int data;
}v;
void isr(int num)
{
	printf("recived signal:%d\n",num);
}
int main(int argc,char*argv[])
{
	int id=msgget(7,IPC_CREAT|0664);
	if(msgget<0){perror("msgget");return 0;}
	if(argc!=3){printf("a.out a b\n");return 0;}
	signal(SIGUSR1,isr);    //applicable for all processes..ie including ch_processes    
	int r1,r2,r3,r4,a=atoi(argv[1]),b=atoi(argv[2]);
	if((r1=fork())==-1){perror("fork");return 0;}
	if(r1>0)
	{
		pause();
		msgrcv(id,&v,sizeof(int),1,0);
		printf("add:%d\n",v.data);
		if((r2=fork())==-1){perror("fork");return 0;}
		if(r2>0)
		{
			sleep(1);
			kill(r2,SIGUSR1);
			pause();
			msgrcv(id,&v,sizeof(int),2,0);
			printf("sub:%d\n",v.data);
			if((r3=fork())==-1){perror("fork");return 0;}
			if(r3>0)
			{
				sleep(1);
				kill(r3,SIGUSR1);
				pause();
				msgrcv(id,&v,sizeof(int),3,0);
				printf("mul:%d\n",v.data);
				if((r4=fork())==-1){perror("fork");return 0;}
				if(r4>0)
				{
					sleep(1);
					kill(r4,SIGUSR1);   
					pause();
					msgrcv(id,&v,sizeof(int),4,0);
					printf("div:%d\n",v.data);
				}
				else
				{
					pause();
					printf("div process pid:%d  a:%d b:%d\n",getpid(),a,b);
					v.mtype=4;
					if(b==0){v.data=0;}
					else{v.data=(a/b);}
					msgsnd(id,&v,sizeof(int),0);
					kill(getppid(),SIGUSR1);
					exit(0);
				}
			}
			else
			{
					pause();
					printf("mul process pid:%d\n",getpid());
					v.mtype=3;
					v.data=a*b;
					msgsnd(id,&v,sizeof(int),0);
					kill(getppid(),SIGUSR1);
					exit(0);
			}

		}
		else
		{
					pause();
					printf("sub process pid:%d\n",getpid());
					v.mtype=2;
					v.data=a-b;
					msgsnd(id,&v,sizeof(int),0);
					kill(getppid(),SIGUSR1);
					exit(0);
		}
	}
	else
	{
					printf("add process pid:%d\n",getpid());
					v.mtype=1;
					v.data=a+b;
					msgsnd(id,&v,sizeof(int),0);
					kill(getppid(),SIGUSR1);
					exit(0);
	}
}
