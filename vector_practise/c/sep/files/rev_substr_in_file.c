#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void rev_substr_in_file(FILE *fp,char **argv) {
	if(fp == NULL) {
		printf("%s no such file found",argv[2]);
		exit(0);
	}
	char str[100];
	char *p = NULL;
	int len = strlen(argv[1]);
	int count = 0;
	while((fgets(str,100,fp)) != NULL) {
		p = str;
		while(p = strstr(p,argv[1])) {
			count++;
			p += len;
		}
	}
	printf("%d\n",count);
	fclose(fp);
}

int main(int argc,char *argv[]) {
	FILE *fp;
	if(argc != 3) {
		printf("Invalid inputs");
		exit(0);
	}
	fp = fopen(argv[2],"r");
	rev_substr_in_file(fp,argv);
}


