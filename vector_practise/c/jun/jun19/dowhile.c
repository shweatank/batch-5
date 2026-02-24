#include <stdio.h>
#include <unistd.h>
int main() {
	char ch = 'A';
	do {
		printf("%c,",ch++);
		fflush(stdout);
		sleep(1);
	}while(ch <= 'Z');
}
