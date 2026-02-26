#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
int main()
{
	int fd;
	fd=open("/dev/BASIC_CHAR",O_RDONLY);
	if(fd<0)
	{
		perror("open");
		return 0;
	}
	close(fd);
}
