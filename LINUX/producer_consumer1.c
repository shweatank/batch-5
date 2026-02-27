#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
int main()
{
	int pfd[2],ret;
	if(pipe(pfd)==-1)
	{
		perror("pipe");
		return 0;
	}
	if((ret=fork())==-1)
	{
		perror("fork");
		return 0;
	}
	if(ret>0)
	{
		close(pfd[0]);
		for(int i=1;i<11;i++)
		{
			write(pfd[1],&i,sizeof(int));
			sleep(1);
		}
		printf("produced\n");
	}
	else
	{
		if((mkfifo("prod",0666))==-1)
		{
			perror("mkfifo");return 0;
		}
		int fdw=open("prod",O_WRONLY);
		int x;
		close(pfd[1]);
		while((read(pfd[0],&x,sizeof(int)))>0)
		{
			printf("%d\n",x);
			x*=x;
			write(fdw,&x,sizeof(int));
		}
		puts("consumed");
	}
}



