#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

int main(){
	char tx[]="k",rx[10];
	char ch2='A';
	int fd=open("/dev/rpi_uart",O_RDWR);
	
	//for(int i=0;tx[i];i++){
		write(fd,&ch2,1);
	printf("data sent=%c\n",ch2);

}
