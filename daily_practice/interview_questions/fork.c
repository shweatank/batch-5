#include<stdio.h>
#include<unistd.h>

int x;

int main()
{
	int ret;
	ret=fork();
	if(ret<0)
	{
		perror("fork");
		return 0;
	}
	if(ret==0)
	{
		x=10;
		printf("in child %d \n",x);
	}
	else
	{
		//x=20;
		printf("in parent %d\n",x);
	}
}

