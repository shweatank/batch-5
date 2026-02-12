#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

int main(){
	char tx[]="sivalsg",rx[10];
	char ch2;
	int fd=open("/dev/rpi_uart",O_RDWR);
	
	for(int i=0;tx[i];i++){
		write(fd,tx+i,1);
	//	perror("write");
		read(fd,rx+i,1);
	//	perror("read");
//		printf("%c\n",ch2);
	}
	ch2='S';
	printf("%s\n",rx);
	sleep(1);
	write(fd,&ch2,1);
	read(fd,&ch2,1);
}

