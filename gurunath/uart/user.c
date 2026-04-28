#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
void main()
{
	int fd=open("/dev/my_uart",O_RDWR);
	perror("open");


	char s[]="gurunath",d[20];

	write(fd,s,strlen(s));

//	for(int i=0;i<10;i++)
//	{
		//read(fd,d,sizeof(d));

		//printf("%s\n",d);
//	}
}
