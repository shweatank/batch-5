#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
	FILE *fp;
	fp = fopen("files.txt","w+");
	if(fp == NULL) {
		perror("fopen\n");
		return 0;
	}
	char str[100] = "This is demo text for testing files calls";
	fwrite(str,strlen(str)+1,strlen(str)+1,fp);
	perror("fwrite\n");
	
	memset(str,0,100);
	fseek(fp,0,SEEK_SET);
	fread(str,30,30,fp);
	perror("fread\n");
	printf("The text read from file is: %s\n",str);
	fclose(fp);
}
