#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	int p,count = 0;
	p=fork();
	p=fork();
	p=fork();
	p=fork();
	if(p != 0) {
		while(wait(NULL) > 0) {
			count++;
		}
		printf("%d",count);
	}
}
