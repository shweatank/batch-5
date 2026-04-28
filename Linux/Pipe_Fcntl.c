
#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
int main()
{
	int pipefd[2];
	int size=0;
	if(pipe(pipefd)==-1)
	{
		perror("pipe");
		return 0;
	}
	size=fcntl(pipefd[0],F_GETPIPE_SZ);
	if(size==-1)
	{
		perror("fcntl");
		return 0;
	}
	printf("size is %d\n",size);
}
