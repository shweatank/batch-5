#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
int main()
{
	int fd[2],ret=0;
	char str[25],sub[25];
	if((pipe(fd))==-1)
	{
		perror("pipe");
		return 0;
	}
	if((ret=fork())==-1)
	{
		perror("fork");
		return 0;
	}
	if(ret>0)
	{
		close(fd[0]);
		while(1)
		{
			fgets(str,25,stdin);if(str[strlen(str)-1]==10)str[strlen(str)-1]=0;
			write(fd[1],str,strlen(str)+1);
			printf("writing done succesfully\n");
		}
	}
	else
	{
		close(fd[1]);
		while(1)
		{
			read(fd[0],sub,sizeof(sub));
			printf("read succesfully from parent\n");
			puts(sub);
		}
	}

}
