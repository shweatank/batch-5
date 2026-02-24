#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
	int stat;
	int fd=open("/dev/led_gpio",O_RDWR);
	if(fd<0)
	{
		perror("open");
		exit(0);
	}
	while(1)
	{
		read(fd,&stat,4);
		if(stat==1)
		{
			write(fd,"1",1);
			sleep(1);
			write(fd,"0",1);
			sleep(1);
		}
	}
	return 0;
}
