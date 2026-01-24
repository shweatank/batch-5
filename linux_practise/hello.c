#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
	for(int i = 0;i<10;i++) {
		sleep(3);
		if(fork() == 0) {
			printf("%d\n",getpid());
			exit(0);
		}
	}
}
