#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

int main(){
	char ch='S';
	char ch2;
	int fd=open("/dev/rpi_uart",O_RDWR);
	write(fd,&ch,1);
	perror("write");
	read(fd,&ch2,1);
	perror("read");
	close(fd);
}

