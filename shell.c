#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
int main()
{
	int r=fork();
	//printf("hello world\n");
	if(r==0)
	{
		execl("./e1","e1",NULL);
		exit(0);
	}
	else
	{
		sleep(1);
		printf("hello\n");
	}
}
