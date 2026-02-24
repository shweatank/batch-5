#include <stdio.h>
#include <sys/resource.h>

void demo();
int main() {
	struct rlimit lim;
	getrlimit(RLIMIT_STACK,&lim);
	lim.rlim_cur = 4096;
	lim.rlim_max = 4096;
	setrlimit(RLIMIT_STACK,&lim);
	demo();
	printf("\n%lu\n",lim.rlim_cur);
}

void demo() {	
	int cnt = 0;
	char str[5000];
	while(cnt <= 4999) {
		str[cnt] = 'a';
		printf("%c ",str[cnt++]);
	}
}
