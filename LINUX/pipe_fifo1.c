#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
int main()
{
	int pfd[2],ret=0,ret2=0,fdr,fdw,mul,pidr,pid;
	if(pipe(pfd)==-1){perror("pipe");return 0;}
	if((ret=fork()) ==-1)
	{
		perror("fork");
		return 0;
	}
	if(ret>0)
	{
		if((ret2=fork()) ==-1)
		{
			perror("fork");
			return 0;
		}
		if(ret2>0)
		{
		if(mkfifo("pro",0666)==-1)
		{
			perror("mkfifo");
			return 0;
		}
		fdr=open("pro",O_RDONLY);
		if(read(fdr,&mul,sizeof(int))==0)
		{
			printf("EOF\n");
			return 0;
		}
		printf("parent read from fifo\n");
		printf("child1 multified pid=%d\n",mul);
		}
		else
		{
			close(pfd[1]);
			read(pfd[0],&pidr,sizeof(int));
			printf("child2 read pid of child 1 from pipe\n");
			printf("child 1 pid:%d\n",pidr);
			fdw=open("pro",O_WRONLY);
			pidr*=2;
			write(fdw,&pidr,sizeof(int));
			printf("child2 multiplied pid of ch1 and written to fifo\n");
		}
	}
	else
	{
		close(pfd[0]);
		int pi=getpid();
		write(pfd[1],&pi,sizeof(int));
		printf("child1 written pid to pipe\n");
	}
}
/*
Child 1 writes its PID to pipe

Child 2 reads PID from pipe → writes 2×PID to FIFO

Parent reads 2×PID from FIFO*/
