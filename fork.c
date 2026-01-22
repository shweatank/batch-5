#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
int main()
{
	int ret=fork();
	int status;
	if(ret==-1)
	{
		perror("fork");
		return 0;
	}
	if(ret==0)
	{
		execl("./cal.out","cal.out",NULL);
	}
	else
	{
		waitpid(ret,&status,0);
	}

	printf("hello word\n");


}
