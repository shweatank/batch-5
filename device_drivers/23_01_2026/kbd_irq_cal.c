#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

struct st{
	int a;
	int b;
};


int main(){
	struct st s={1,2};
	int fd=open("/dev/irq_calc",O_CREAT|0666);
	write(fd,&s,sizeof(s));		
}





