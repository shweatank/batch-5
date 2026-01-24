#include <stdlib.h>
#include <unistd.h>

int main() {
	for(int i = 1;i<=10;i++) {
		if(fork() == 0) {
			system("./a.out");
			exit(0);
		}
	}
}
