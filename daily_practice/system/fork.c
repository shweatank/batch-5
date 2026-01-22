#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
int main()
{
	int ret;
	int status;
	ret=fork();
	if(ret==-1)
	{
		perror("fork");
		return 0;
	}
	if(ret==0)
	{
		printf("process id is %d\n",getpid());
		printf("parent process is %d\n",getppid());
	}
	else
	{
		waitpid(ret,&status,0);
	}
}
