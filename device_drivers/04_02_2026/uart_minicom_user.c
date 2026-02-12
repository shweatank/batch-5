#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

int main(){
	char s[100]="abcd";
	int fd=open("/dev/rpi_uart",O_WRONLY|0777);
	//while(1){
	//	if(read(fd,&s,1)==1)
	//	printf("%c\n",s);
	//}
	write(fd,s,strlen(s)+1);
	printf("writing done\n");
	close(fd);
}


