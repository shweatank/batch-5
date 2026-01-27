#include<stdio.h>
#include<string.h>
#include<unistd.h>
int main()
{
	int fd[2];
	int n=pipe(fd);
	if(n<0)
	{
		perror("pipe");
		return 0;
	}
	int ret;
	char str[100];
	ret=fork();
	if(ret==-1)
	{
		perror("fork");
		return 0;
	}
	if(ret!=0)
	{
		close(fd[0]);
		puts("enter the string");
		scanf("%[^\n]s",str);
		write(fd[1],str,strlen(str));
		close(fd[1]);
	}
	else
	{
		close(fd[1]);
		memset(str,0,sizeof(str));
		read(fd[0],str,sizeof(str));
		printf("recived string from parent is %s\n",str);
		close(fd[0]);

	}
}

