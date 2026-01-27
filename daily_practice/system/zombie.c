#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main()
{
	int ret;
	ret=fork();
	if(ret==-1)
	{
		perror("fork");
		return 0;
	}
	if(ret!=0)
	{
		sleep(10);
	}
	else
	{
		exit(0);
	}
}
