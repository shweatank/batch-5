#include"header.h"
int add(int,int);
struct msg
{
	long mtype;
	int res;
}m,m2;
int id;
int n1=30,n2=20;
void ISR(int n)
{
	if(n==SIGUSR1)
	{
		msgrcv(id,&m2,sizeof(m2.res),1,0);
		printf("add=%d\n",m2.res);
	}
	else if(n==SIGUSR2)
	{
		msgrcv(id,&m2,sizeof(m2.res),2,0);
		printf("subtarction=%d\n",m2.res);
	}
	else if(n==SIGHUP)
	{
		msgrcv(id,&m2,sizeof(m2.res),3,0);
		printf("mul=%d\n",m2.res);
	}
	else if(n==SIGPIPE)
	{
		msgrcv(id,&m2,sizeof(m2.res),4,0);
		printf("division=%d\n",m2.res);
	}
}
void main()
{
	id=msgget(id,IPC_CREAT|0664);
	if(id<0)
	{
		perror("msgget");
		return;
	}
	if(fork()==0)
	{ 
		while(1)
		{
			sleep(1);
			m.res=add(n1,n2);
			m.mtype=1;
			msgsnd(id,&m,sizeof(m.res),0);
			printf("child send the data into msg queue mtype 1\n");
			kill(getppid(),SIGUSR1);

		}	}
	else
	{
		if(fork()==0)
		{
			while(1)
			{
				sleep(2);
				m.res=sub(n1,n2);
				m.mtype=2;
				msgsnd(id,&m,sizeof(m.res),0);
				printf("child send the data into msg queue mtype 2\n");
				kill(getppid(),SIGUSR2);
			}
		}
		else
		{
			if(fork()==0)
			{
				while(1)
				{
					sleep(3);
					m.res=multiply(n1,n2);
					m.mtype=3;
					msgsnd(id,&m,sizeof(m.res),0);
					printf("child send the data into msg queue mtype 3\n");
					kill(getppid(),SIGHUP);
				}
			}
			else
			{
				if(fork()==0)
				{
					while(1)
					{
						sleep(4);
						m.res=division(n1,n2);
						m.mtype=4;
						msgsnd(id,&m,sizeof(m.res),0);
						printf("child send the data into msg queue mtype 4\n");
						kill(getppid(),SIGPIPE);
					}
				}
				else
				{
					struct sigaction v;
					v.sa_handler=ISR;
					v.sa_flags=0;
					sigemptyset(&v.sa_mask);
					sigaction(SIGUSR1,&v,0);
					sigaction(SIGUSR2,&v,0);
					sigaction(SIGHUP,&v,0);
					sigaction(SIGPIPE,&v,0);
					while(1)
					{
						printf("main will pause until get signal\n");
						pause();
					}
				}
			}
		}
	}
}
