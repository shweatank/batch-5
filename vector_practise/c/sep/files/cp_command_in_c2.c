#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
	FILE *fp,*dest;
	char ch;
	char str[100];
	fp = fopen("cpsrc2.txt","r");
	if(fp == NULL) {
		printf("file not found");
		exit(0);
	}
	dest = fopen("cpdest2.txt","w");
	while(fgets(str,100,fp)) {
		fputs(str,dest);
	}
	fclose(fp);
	fclose(dest);
}

