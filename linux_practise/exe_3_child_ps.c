#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>


int main() {
	int c1,c2,c3;

	c1 = fork();
	if(c1 == 0) {
		sleep(5);
		printf("This is child 1\n");
		printf("pid: %d  ppid: %d\n",getpid(),getppid());
		printf("child 1 is terminated after 5 sec delay\n");
		exit(0);
	}

	c2 = fork();
	if(c2 == 0) {
		sleep(9);
		printf("This is child 2\n");
		printf("pid: %d  ppid: %d\n",getpid(),getppid());
		printf("child 2 is terminated after 9 sec delay\n");
		exit(0);
	}

	c3 = fork();
	if(c3 == 0) {
		sleep(4);
		printf("This is child 3\n");
		printf("pid: %d  ppid: %d\n",getpid(),getppid());
		printf("child 3 is terminated after 4 sec delay\n");
		exit(0);
	}

	printf("Waiting for child 3 to get terminated\n");
	while(wait(NULL) != c3);
	printf("child 3 is terminated and main terminated\n");
	sleep(30);
}

