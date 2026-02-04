#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>

struct data
{
	int d1;
	int d2;
	char ch;
};

int main()
{
	struct data d;
	d.d1=20;
	d.d2=30;
	d.ch='A';
	int fd=open("dev/key_int_usr",O_RDWR);
	if(fd<0)
	{
		perror("open");
		exit(0);
	}
	write(fd,&d,sizeof(d));
	close(fd);
}
