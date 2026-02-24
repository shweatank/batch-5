#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc,char *argv[]) {
	if(argc < 2) {
		printf("Invlaid Arguments..");
		return 0;
	}
	char cmd[20];
	for(int i = 1;i<argc;i++) {
		strcat(cmd,argv[i]);
		strcat(cmd," ");
	}
	system(cmd);
}
