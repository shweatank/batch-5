#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

//jobs executing concurrently
int main(int argc,char *argv[]) {
	if(argc < 2) {
		printf("Invalid Arguments..\n");
		return 0;
	}
	for(int i = 1;i<argc;i++) {
		if(fork() == 0) {
			system(argv[i]);
			exit(0);
		}
	}
	while(wait(NULL) > 0);
	/* jobs executing sequential
	 for(int i = 1;i<argc;i++) {
	 	system(argv[i]);
	}
	*/
}

