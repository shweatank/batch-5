#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main()
{
	int ret=0,ret1=0,ret2=0;
	int a=5,b=3,c=0;
	ret=fork();
	c=a+b;
	if(ret==-1)
	{
		printf("child not created\n");
		return 0;
	}
	if(ret==0)
	{
		printf("pid:%d ppid:%d c=%d\n",getpid(),getppid(),c);
		exit(0);
	}
	else
	{
		printf("parent pid:%d parent ppid:%d c=%d\n",getpid(),getppid(),c);
		ret1=fork();
		if(ret1==-1)
		{
			printf("child not created\n");
			return 0;
		}
		if(ret1==0)
		{
			printf("zombie...pid:%d ppid:%d\n",getpid(),getppid());
			printf("zombie ended\n");
			exit(0);
		}
		else
		{	
			printf("parent pid:%d parent ppid:%d c=%d\n",getpid(),getppid(),c);
			sleep(5);
			ret2=fork();
			if(ret2==0)
			{
			printf("orphan...pid:%d ppid:%d\n",getpid(),getppid());
			printf("this is going to be orphan\n");
			sleep(12);
			printf("orphan ended\n");
			}
			else
			{
				printf("parent pid:%d parent ppid:%d c=%d\n",getpid(),getppid(),c);
				printf("parent ended\n");
				exit(0);
			}
		}	
	}
}
