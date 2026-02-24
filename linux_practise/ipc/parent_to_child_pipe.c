#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main() {
	int pipefd[2];
	char sdata[100];
	pipe(pipefd);
	perror("pipe");
	if(fork() == 0) {
		char rdata[100];
		pipefd[1];	//write end is closed
		read(pipefd[0],&rdata,100);
		printf("In child process\n");
		printf("Received string is: %s\n",rdata);
		exit(0);
	}
	else {
		puts("In parent process");
		printf("Enter the string to be sent: ");
		scanf("%s",sdata);
		close(pipefd[0]);	//read end closed
		write(pipefd[1],&sdata,strlen(sdata)+1);
	}
	while(wait(NULL) > 0);
}

