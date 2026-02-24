#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int lines,words,chars;
void mywccommand(char *filename) {
	FILE *fp = fopen(filename,"r");
	if(fp == NULL) {
		printf("File not found");
		exit(0);
	}
	char str[100];
	while(fgets(str,100,fp) != NULL) {
		lines++;
		int i = 0,len = strlen(str);
		chars+=len;
		int flag = 0;
		while(i < len) {
			if(isspace(str[i])) flag = 0;
			else if(!flag) {
				flag = 1;
				words++;
			}
			i++;
		}
	}
}

int main(int argc,char *argv[]) {
	if(argc != 2) {
		printf("Invalid arguments\n");
		exit(0);
	}
	mywccommand(argv[1]);
	printf("%d %d %d %s\n",lines,words,chars,argv[1]);
}
