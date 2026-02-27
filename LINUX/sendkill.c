#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main()
{
	printf("%d %d\n",getpid(),getppid());
	while(1);
}
//sent=>    kill -9 pid
