#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

struct val
{
	int a;
	int b;
};
void main()
{
	struct val v;
	v.a=10;
	v.b=20;

	int fd;

	fd=open("/dev/int_dev",O_RDWR);
	if(fd<0)
	{
		perror("open");
		return;
	}


