#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
int main()
{
	int fd,fd2;
	fd=open("sai.txt",O_RDONLY,666);
	if(fd<0)
	{
		perror("open");
		return 0;
	}
	fd2=open("copy.txt",O_CREAT|O_WRONLY|O_TRUNC,666);
	if(fd2<0)
	{
		perror("open");
		return 0;
	}
	char str[100];
	size_t n;
	while((n=read(fd,str,sizeof(str)))!=0)
	{
		write(fd2,str,n);
	}
	close(fd);
	close(fd2);
}
