#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/shm.h>
#include<stdio_ext.h>
void isr(int num)
{
	printf("recived signal:%d\n",num);
}
int main(int argc,char*argv[])
{
	int id=shmget(7,40,IPC_CREAT|0664);
	int *p=NULL;
	if(id<0){perror("msgget");return 0;}
	if(argc!=3){printf("a.out a b\n");return 0;}
	//signal(SIGUSR1,isr);        
	int r1,r2,r3,r4,a=atoi(argv[1]),b=atoi(argv[2]);
	if((r1=fork())==-1){perror("fork");return 0;}
	if(r1>0)
	{
		signal(SIGUSR1,isr);
		pause();
		p=shmat(id,0,SHM_RDONLY);
		printf("add:%d\n",*p);
		shmdt(p);
		if((r2=fork())==-1){perror("fork");return 0;}
		if(r2>0)
		{
			sleep(1);
			kill(r2,SIGUSR1);
			signal(SIGUSR1,isr);       
			pause();
			p=shmat(id,0,SHM_RDONLY);
			printf("sub:%d\n",*p);
			shmdt(p);
			if((r3=fork())==-1){perror("fork");return 0;}
			if(r3>0)
			{
				sleep(1);
				kill(r3,SIGUSR1);
				signal(SIGUSR1,isr);        //registers for all including ch process
				pause();
				p=shmat(id,0,SHM_RDONLY);
				printf("mul:%d\n",*p);
				shmdt(p);
				if((r4=fork())==-1){perror("fork");return 0;}
				if(r4>0)
				{
					//sleep(1);
					kill(r4,SIGUSR1);
					signal(SIGUSR1,isr);       
					pause();
					p=shmat(id,0,SHM_RDONLY);
					printf("div:%d\n",*p);
					shmdt(p);
					shmctl(id,IPC_RMID,NULL);
				}
				else
				{
					printf("div process pid:%d\n",getpid());
					p=shmat(id,0,0);
					*p=(a/b);
					shmdt(p);
					kill(getppid(),SIGUSR1);
					exit(0);
				}
			}
			else
			{
					signal(SIGUSR1,isr);        
					pause();
					printf("mul process pid:%d\n",getpid());
					p=shmat(id,0,0);
					*p=a*b;
					shmdt(p);
					kill(getppid(),SIGUSR1);
					exit(0);
			}

		}
		else
		{
					signal(SIGUSR1,isr);        
					pause();
					printf("sub process pid:%d\n",getpid());
					p=shmat(id,0,0);
					*p=a-b;
					shmdt(p);
					kill(getppid(),SIGUSR1);
					exit(0);
		}
	}
	else
	{
					printf("add process pid:%d\n",getpid());
					p=shmat(id,0,0);
					*p=a+b;
					shmdt(p);
					kill(getppid(),SIGUSR1);
					exit(0);
	}
}
