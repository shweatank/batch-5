#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
int main()
{
	int fd;
	fd=open("/dev/char_device",O_RDWR);
	char str[100];
	write(fd,str,100);
	read(fd,str,100);
	close(fd);
}
