//Write a program in which a child process replaces itself using exec(). The parent must detect child termination using a SIGCHLD handler instead of calling wait() directly.

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

int child_exited;
void sighandler(int dummy) {
	pid_t pid;
	int status;
	printf("In signal handler\n");
	while((pid = waitpid(-1,&status,WNOHANG)) > 0) {
		if(WEXITSTATUS(status)) {
			printf("Child %d terminated with status of %d\n",pid,WEXITSTATUS(status));
		}
	}
	child_exited = 1;
}

int main(int argc,char **argv) {
	if (argc > 1 && strcmp(argv[1], "child") == 0) {
        	printf("Exec-replaced child process: PID=%d\n", getpid());
		exit(5);
    	}
	struct sigaction sa;
	sa.sa_handler = sighandler;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGCHLD,&sa,NULL);
	pid_t pid = fork();
	if(pid == 0) {
		printf("In child process\n");
		execl(argv[0],argv[0],"child",NULL);
		perror("Exec\n");
		exit(1);
	}
	while(!child_exited) pause();
}
