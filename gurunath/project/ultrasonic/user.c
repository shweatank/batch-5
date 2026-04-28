#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

void main()
{

	char s[100];

	while(1)
	{
		//int fd1=open("/dev/ili9225_char",O_RDWR);

		int fd2=open("/proc/hcsr04_direction_led",O_RDONLY);

		read(fd2,s,sizeof(s));
	        sleep(1);
	//	write(fd1,s,strlen(s)+1);
		printf("%s",s);

		//close(fd1);
		close(fd2);
	}

}

