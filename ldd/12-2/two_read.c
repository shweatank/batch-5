#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

int main(){
	char tx[]="sivalsg",rx[15];
	int fd=open("/dev/rpi_uart",O_RDWR);
int i=0;
while(1)
{
sleep(1);
		read(fd,rx+i,1);
i++;
	printf("%c\n",rx[i]);
if(rx[i]=='\0')
break;
}
	close(fd);
}
