#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
	FILE *fp,*dest;
	char ch;
	int count = 0;
	fp = fopen("cp_command_in_c.c","r");
	if(fp == NULL) {
		printf("file not found");
		exit(0);
	}
	dest = fopen("cp_command_in_c2.c","w");
	while((ch = fgetc(fp)) != EOF) {
		fputc(ch,dest);
		count++;
	}
	printf("size of destination file is : %d\n",count);
	fclose(fp);
	fclose(dest);
}

