#include<stdio.h>
#include<fcntl.h>
int main()
{
	int fd;
	fd=open("/dev/rpi_uart",O_RDWR);

