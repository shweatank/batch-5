//Write a program where a parent creates 3 child processes. Each child sends its PID to the parent using a pipe, and the parent waits for all children and prints their exit order.

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
	pid_t pid,exit_pid;
	int pipefd[2];
	pipe(pipefd);
	int i;
	for(i = 0;i<3;i++) {
		pid = fork();
		if(pid == 0) {
			close(pipefd[0]);
			pid_t cpid = getpid();
			write(pipefd[1],&cpid,sizeof(cpid));
			close(pipefd[1]);
			sleep(i+1);
			exit(i+1);;
		}
	}
	close(pipefd[1]);
	for(i = 0;i<3;i++) {
		pid_t rpid;
		read(pipefd[0],&rpid,sizeof(rpid));
		printf("The value written in pipe by child is: %d\n",rpid);
	}
	close(pipefd[0]);
	while((exit_pid = wait(NULL)) > 0) {
		printf("Child exited ;%d\n",exit_pid);
	}
	return 0;
}
