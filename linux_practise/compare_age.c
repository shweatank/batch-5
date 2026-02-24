#include <stdio.h>
#include <sys/stat.h>

int main(int argc,char **argv) {
	if(argc != 3) {
		printf("Invalid Arguments..!\n");
		return 0;
	}
	struct stat s1,s2;
	if((stat(argv[1],&s1) || stat(argv[2],&s2)) == -1) {
		printf("Unable to open the path..!\n");
		return 0;
	}
	if (s1.st_mtim.tv_sec > s2.st_mtim.tv_sec ||
	   (s1.st_mtim.tv_sec == s2.st_mtim.tv_sec &&
	    s1.st_mtim.tv_nsec > s2.st_mtim.tv_nsec)) {
		printf("%s is modified newly\n",argv[1]);
	}
	else {
		printf("%s is modified newly\n",argv[2]);
	}
}

