#include<stdio.h>
#include<unistd.h>
int main()
{
	while(1)
	{
		printf("pid:%d\n",getpid());
		//printf("hello world\n");
	}
}
