#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
int main(){
	pid_t p;
	int n,a=10,b=5,sum,fd;
	puts("Enter receiver PID:");
	scanf("%d",&p);
	//kill(p,SIGUSR1);
	puts("signal sent");
	n=mkfifo("fifo",0666);
	fd=open("fifo",O_WRONLY);
	sum=a+b;
	write(fd,&sum,sizeof(int));
	printf("wrote sum %d into sum\n",sum);
        kill(p,SIGUSR1);	
	close(fd);
}

