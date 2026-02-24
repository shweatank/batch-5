#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv) {
	int min = atoi(argv[1]);
	int max = atoi(argv[2]);
	int reduce = atoi(argv[3]);
	for(min;min<max;min++) {
		int num = min;
	loop:	int n = 0;
		while(num) {
			n += num % 10;
			num /= 10;
		}
		if(n > 9) {
			num = n;
			goto loop;
		}
		else if(n == reduce) {
			printf("%d  ",min);
		}
	}
}


