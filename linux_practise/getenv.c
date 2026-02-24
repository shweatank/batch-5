#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv) {
	if(argc != 2) {
		perror("Invlaid Arguments.. ");
		return 0;
	}
	char * path = getenv(argv[1]);
	printf("%s\n",path);
}

