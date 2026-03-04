#include<stdio.h>
#include<unistd.h>
#include<string.h>
int main()
{
	int fd[2];
	char str[100],dst[100];
	int ret=pipe(fd);
	if(ret<0)
	{
		perror("pipe");
		return 0;
	}
	ret=fork();
	if(ret<0)
	{
		perror("fork");
		return 0;
	}
	else if(ret==0)
	{
		close(fd[1]);
		read(fd[0],dst,sizeof(dst));
		printf("message from the parent is %s\n",dst);
		close(fd[0]);
	}
	else
	{
		close(fd[0]);
		puts("enter the string");
		fgets(str,100,stdin);
		write(fd[1],str,strlen(str)+1);
		close(fd[1]);
	}
}
