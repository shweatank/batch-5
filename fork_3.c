#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int global_x;
int main()
{
	int local_y=10;
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
		printf("Global variable: %d and local variable: %d\n",global_x,local_y);
		printf("Address of x: %ld and y: %d\n",&global_x,&local_y);
	//	exit(0);
	}
	else
	{
		printf("Parent process:\n");
		printf("pid=%d ppid()=%d\n",getpid(),getppid());
		printf("Global variable: %d and local variable: %d\n",global_x,local_y);
		printf("Address of x: %ld and y: %d\n",&global_x,&local_y);
	//	exit(0);
	}
}
