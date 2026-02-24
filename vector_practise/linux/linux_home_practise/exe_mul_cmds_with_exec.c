#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc,char **argv) {
	
	if(argc < 2) {
		printf("Invalid Arguments..");
		return 0;
	}
	/*
	for(int i = 1;i<argc;i++) {
		if(fork() == 0){
			execlp(argv[i],"demo",NULL);
			exit(0);
		}
	}
	while(wait(NULL) > 0);
	*/
	
	//For sequential execution
	for(int i = 1;i<argc;i++) {
		if(fork() == 0) {
			execlp(argv[i],"demo",NULL);
			exit(0);
		}
		else {
			wait(NULL);
		}
	}
}

