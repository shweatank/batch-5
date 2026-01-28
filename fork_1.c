#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int x;
int main()
{
	pid_t pid=fork();
	if(pid==-1)
	{
		perror("fork");
		return EXIT_FAILURE;
	}
	if(pid==0)
	{
		printf(" Child process:\n");
		printf("pid=%d\n",getpid());
		printf("Global variable: %d\n",x);
		printf("Global variable: %ld\n",&x);
	//	exit(0);
	}
	else
	{
		printf("Parent process:\n");
		printf("pid=%d\n",getpid());
		printf("Global variable: %d\n",x);
		printf("Global variable: %ld\n",&x);
	//	exit(0);
	}
}
