#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
int main()
{
	int ret,status;
	for(int i=0;i<3;i++)
	{
		ret=fork();
		if(ret==-1)
		{
			perror("fork");
			return 0;
		}
		if(ret==0)
		{
			printf("process id is %d\n",getpid());
			printf("parent process id is %d\n",getppid());
			exit(0);
		}
	}
	for(int i=0;i<3;i++)
		wait(&status);

}
