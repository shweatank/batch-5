#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc,char ** argv) {
	if(argc != 2) {
		printf("Invlaid Arguments..");
		return 0;
	}
	int n = atoi(argv[1]);
	for(int i = 0;i<n;i++) {
		if(fork() == 0) {
			printf("child%d, pid: %d , ppid: %d\n",i+1,getpid(),getppid());
			exit(0);
		}
	}
	printf("parent process: %d , %d\n",getpid(),getppid());
}
