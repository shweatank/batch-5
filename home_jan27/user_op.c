#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>

struct data
{
	int n1,n2;
	char ch;
};

int main()
{
	struct data d;
	int res=0;
	int fd=open("/dev/keyboard_int_usr",O_RDWR);
	if(fd<0)
	{
		perror("open");
		exit(0);
	}
	write(fd,&d,sizeof(d));
	getchar();
	read(fd,&res,sizeof(res));
	return 0;
}
