#include <stdio.h>
#include <sys/resource.h>
#include <unistd.h>
#include <signal.h>

int main() {
	signal(SIGXFSZ, SIG_IGN);
	int count = 0;
	struct rlimit limits;
	getrlimit(RLIMIT_FSIZE,&limits);
	limits.rlim_cur = 4097;
	limits.rlim_max = 4097;
	setrlimit(RLIMIT_FSIZE,&limits);
	printf("%ld\n",limits.rlim_max);
	FILE *fp = fopen("getrlimit_demo","w");
	while(1) {
		if(fputc('a',fp) == EOF) break;
		count++;
	}
	printf("%d\n",count);
}
