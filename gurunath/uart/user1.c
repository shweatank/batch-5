#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
void main()
{
	int fd=open("/dev/my_uart",O_RDWR);
	perror("open");


	char s[]="gurunath",d[20];

             read(fd,d,sizeof(s));

	     printf("%s\n",d);

}
