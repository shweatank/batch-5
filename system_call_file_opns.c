#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
int main()
{
	char str[100],ptr[100];scanf("%s",str);
	int fd=0;
	fd=open("dummy.txt",O_WRONLY|O_TRUNC);
	write(fd,str,strlen(str)+1);
	close(fd);
	fd=open("dummy.txt",O_RDONLY);
	read(fd,ptr,sizeof(ptr));
	puts(ptr);
	close(fd);
}
