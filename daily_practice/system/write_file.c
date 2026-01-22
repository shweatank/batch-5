#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
int main()
{
	int fd;
	char str[100];
	puts("enter the string write to file");
	scanf("%[^\n]s",str);
	fd=open("sai.txt",O_CREAT|O_WRONLY,0666);
	if(fd<0)
	{
		perror("open");
		return 0;
	}
	write(fd,str,strlen(str));
	close(fd);
}
