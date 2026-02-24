#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void myCopyCommand(int argc,char *files[]) {
	if(argc < 2) {
		printf("Invalid arguments\n");
		exit(0);
	}
	FILE *fp = fopen(files[1],"r");
	if(fp == NULL) {
		printf("%s Source file not found\n",files[1]);
		exit(0);
	}
	fseek(fp,0,SEEK_END);
	int len = ftell(fp);
	fseek(fp,0,SEEK_SET);
	char str[len];
	fread(str,len,1,fp);
	for(int i = 2;i<argc;i++) {
		FILE *dest = fopen(files[i],"w");
		fwrite(str,len,1,dest);
		fclose(dest);
	}
	fclose(fp);
}

int main(int argc,char *argv[]) {
	myCopyCommand(argc,argv);
}
