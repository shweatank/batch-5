#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio_ext.h>
#include<sys/wait.h>
int add(int x,int y)
{
	exit(x+y);
}
int sub(int x,int y)
{
	exit(x-y);
}
int mul(int x,int y)
{
	exit(x*y);
}
int divv(int x,int y)
{
	if(y==0)
	{
		printf("Invalid data\n");
		return 0;
	}
	exit(x/y);
}
void main()
{
	pid_t pid=fork();
	int status;
	if(pid==-1)
	{
		perror("fork");
		return;
	}

	if(pid==0)
	{
		char ch;
		int a,b;
		printf("Enter the two numbers:\n");
		scanf("%d%d",&a, &b);
		printf("Enter the choice:\n");
		printf("a->add, b->sub, c->mul, d->divv\n");
		scanf(" %c",&ch);
		switch(ch)
		{
			case 'a': add(a,b);break;
			case 'b': sub(a,b); break;
			case 'c': mul(a,b);break;
			case 'd': divv(a,b);break;
			default: printf("Invalid option\n");
		}
		exit(0);
	}
	else
	{
		wait(&status);
		if(WIFEXITED(status))
			printf("Parent: result: %d\n",WEXITSTATUS(status));
	}
}
