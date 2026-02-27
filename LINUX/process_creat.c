#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main()
{
	int ret=0;
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
	}
	else
	{
		printf("parent pid:%d parent ppid:%d c=%d\n",getpid(),getppid(),c);
	}
	return 0;
	
}
