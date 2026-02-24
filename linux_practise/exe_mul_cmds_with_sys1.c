#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char **argv) {
	if(argc < 2) {
		printf("Invalid Arguments..!");
		return 0;
	}
	for(int i = 1;i<argc;i++) {
		if(fork() == 0) {
			system(argv[i]);
			exit(0);
		}
	}
}

