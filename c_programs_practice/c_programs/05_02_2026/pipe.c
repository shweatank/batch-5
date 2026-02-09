#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

int main(){
	int fd[2];
	char s[10];
	pipe(fd);
	if(fork()){
		close(fd[1]);
		printf("parent:%d\n",getpid());
		int n=read(fd[0],s,4);
		printf("%s %d\n",s,n);		
	}
	else{
		printf("child:%d\n",getpid());
		close(fd[0]);
		write(fd[1],"abc",4);
		while(1);
	}
}



