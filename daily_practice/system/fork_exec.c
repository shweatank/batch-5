#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
int main()
{

	int ret,status;
	ret=fork();
	if(ret==-1)
	{
		perror("fork");
		return 0;
	}
	if(ret==0)
	{
		printf("in child\n");
		execl("/bin/ls","ls",NULL);
	}
	else
	{
		waitpid(ret,&status,0);
		printf("child terminated sucessfully\n");
	}
}
