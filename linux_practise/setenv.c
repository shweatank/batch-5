#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv) {
	if(argc != 2) {
		perror("Invlaid Arguments.. ");
		return 0;
	}
	char * path = malloc(10);
	path = "new value";
	setenv(argv[1],path,1);
	printf("%s\n",getenv(argv[1]));
}

