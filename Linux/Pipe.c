#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	int pipefd[2],ret;
	if(pipe(pipefd)==-1)
	{
		perror("pipe");
		return 0;
	}
	ret=fork();
	if(ret==-1)
	{
		perror("fork");
		return 0;
	}
	if(ret)
	{
		char ch[]="abcdef";
		int len=strlen(ch)+1;
		close(pipefd[0]);
		if(write(pipefd[1],ch,len)!=len)
		{
			printf("no of bytes not successful written into pipe\n");
		}
		else
		{
			printf("no of bytes successful\n");
		}

	}
	else
	{
		char a[10],i=0,ch;
		close(pipefd[1]);
		while(1)
		{
		    if(read(pipefd[0],&ch,1))
		    {
			a[i++]=ch;
		    }
		    else
			    break;
		}
		printf("received string:%s\n",a);

	}
}
