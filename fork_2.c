#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main()
{
	int x=10;
	pid_t pid=fork();
	if(pid==-1)
	{
		perror("fork");
		return EXIT_FAILURE;
	}
	if(pid==0)
	{
		printf(" Child process:\n");
		printf("pid=%d ppid=%d\n",getpid(),getppid());
		printf("Global variable: %d\n",x);
	//	exit(0);
	}
	else
	{
		printf("Parent process:\n");
		printf("pid=%d ppid()=%d\n",getpid(),getppid());
		printf("Global variable: %d\n",x);
	//	exit(0);
	}
}
