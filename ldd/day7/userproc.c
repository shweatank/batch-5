#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
void main()
{
	char s[]="10*30";
	int fd=open("/proc/proc_basic",O_RDWR);
	printf("Before :%s\n",s);
	write(fd,s,sizeof(s));
	read(fd,s,sizeof(s));
	printf("After :%s\n",s);
	
}

