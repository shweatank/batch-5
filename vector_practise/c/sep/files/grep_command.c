#include <string.h>
#include <stdlib.h>
#include <stdio.h>
void mygrep(FILE *fp,char **argv) {
	if(fp == NULL) {
		printf("%s no such file found",argv[2]);
		exit(0);
	}
	char str[100];
	while((fgets(str,100,fp)) != NULL) {
		if(strstr(str,argv[1])) {
			printf("%s",str);
		}
	}
	fclose(fp);
}

int main(int argc,char *argv[]) {
	FILE *fp;
	if(argc != 3) {
		printf("Invalid inputs");
		exit(0);
	}
	fp = fopen(argv[2],"r");
	mygrep(fp,argv);
}


