//Write a parent–child program where data is exchanged through a pipe. The parent sends SIGTERM to the child based on a condition, and the child exits gracefully.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

void myHandler(int signo) {
	puts("In child signal handler");
	exit(0);
}

int main() {
	int pipefd[2];
	pipe(pipefd);
	pid_t pid;
	pid = fork();
	char ptr[30];
	if(pid == 0) {
		char p[30] = "Text in child process";
		close(pipefd[0]);
		puts("In child process");
		write(pipefd[1],p,strlen(p)+1);
		signal(SIGTERM,myHandler);
		pause();
	}
	else {
		close(pipefd[1]);
		int ret = read(pipefd[0],ptr,21);
		ptr[21] = '\0';
		printf("%s\n",ptr);
		if(ret == 21) {
			kill(pid,SIGTERM);
		}
	}
}


