#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc,char **argv) {
	int r = fork();
	if(r == 0) {
		int a = atoi(argv[1]);
		int b = atoi(argv[2]);
		int res = a+b;
		exit(res);
	}
	else {
		int res;
		wait(&res);
		printf("%d",WEXITSTATUS(res));
	}
}
