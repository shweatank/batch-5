#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

int main(){
	char tx[]="sivalsg",rx[15];
	char ch2='\0';
	int fd=open("/dev/rpi_uart",O_RDWR);
		read(fd,&ch2,1);
	printf("%c\n",ch2);
	close(fd);
}

