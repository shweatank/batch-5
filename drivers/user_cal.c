#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
void main()
{
	char buf[50];
	int fd=open("/dev/cal",O_RDWR);
	write(fd,"10 20",5);
	read(fd,buf,sizeof(buf));
	printf("%s",buf);
	close(fd);
}
     
