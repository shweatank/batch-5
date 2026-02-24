#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>

int main()
{
	char str[2];
	int fd=open("/dev/gpio_input_led",O_RDWR);
	if(fd<0)
	{
		perror("open");
		exit(0);
	}
	while(1)
	{
		read(fd,str,1);
		if(str[0]=='1')
			write(fd,"1",1);
		else
			write(fd,"0",1);
			
	}
}
