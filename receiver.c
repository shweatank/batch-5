#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

int terminate=0;
void handler(int sig){
	if(sig==SIGUSR1)
		puts("Received SIGUSR1 from another process");
	terminate=1;
}
int main(){
	printf("Received pid %d\n",getpid());
	signal(SIGUSR1,handler);
	int n,sum,fd;
	n=mkfifo("fifo",0666);
	fd=open("fifo",O_RDONLY);
	while(!terminate)
                pause();
	read(fd,&sum,sizeof(int));
	printf("sum is %d\n",sum);
	close(fd);
	//while(!terminate)
	//	pause();
	
}
