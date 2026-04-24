#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

int main(){
	char tx[]="sai",rx[10];
	char ch2;
	int fd=open("/dev/rpi_uart",O_RDWR);
	
	for(int i=0;tx[i];i++){
		write(fd,tx+i,1);
sleep(1);
	printf("data sent=%c\n",tx[i]);
}
}
