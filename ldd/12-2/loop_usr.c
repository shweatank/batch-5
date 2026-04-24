#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

int main(){
	char tx[]="sivalsg",rx[10];
	char ch2;
	int fd=open("/dev/rpi_uart",O_RDWR);
	
	//for(int i=0;tx[i];i++){
		write(fd,"gurunath",8);
	//	perror("write");
	//	read(fd,rx+i,1);
	//	perror("read");
//		printf("%c\n",ch2);
	
}
