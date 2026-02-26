#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
int main()
{
	int fd;
	fd=open("/dev/rpi_uart",O_RDWR);
	if(fd<0)
	{
		perror("open");
		return 0;
	}
	char ch='A';
	write(fd,&ch,1);
	char c;
	read(fd,&c,1);
	printf("%c\n",c);
	close(fd);
}
