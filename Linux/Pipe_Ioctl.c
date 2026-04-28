#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/ioctl.h>


int main()
{
	int pipefd[2];
	int ret;
	if(pipe(pipefd)==-1)
	{
		perror("pipe");
		return 1;
	}
	ret=fork();
	if(ret==-1)
	{
		perror("fork");
		return 1;
	}
	if(ret)
	{
		char a[]="hello";
		int len=strlen(a)+1;
		close(pipefd[0]);
		if(write(pipefd[1],a,len)!=len)
		{
			printf("no of bytes are not successful\n");
		}
		else
			printf("no of write successfully\n");
	}
	else
	{
		close(pipefd[1]);
	        int size;
	        ioctl(pipefd[0],FIONREAD,&size);
	        if(size==-1)
		{
			perror("ioctl");
			return 1;
		}	
		printf("the no of bytes received:%d\n",size);
		char a[size];
		if(read(pipefd[0],a,size)==-1)
		{
			perror("read");
			return 1;
		}
		printf("recevied string:%s\n",a);
			
	}
}
