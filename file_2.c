#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
int main()
{
	char  str[100];
	int fd=open("sai.txt", O_RDONLY);
		read(fd,str,100);
		close(fd);           
	int fd2=open("sai.txt", O_WRONLY);
		write(1,str,strlen(str)+1);
		close(fd2);
}
