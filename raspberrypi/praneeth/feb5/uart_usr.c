#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
int main()
{
	char data='A';
	char ch;
	int i;
	int fd;
	fd=open("/dev/rpi_uart",O_RDWR);
	if(fd<0)
	{
		perror("open");
		exit(0);
	}
	perror("open");
	write(fd,&data,1);
//	read(fd,&ch,1);
	printf("data received : %c\n",ch);
}
