#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void replaceWordWithAnother(char *filename,char *word,char *replacement) {
	FILE *fp = fopen(filename,"r+");
	if(!fp) {
		printf("Invalid filename / Unable to open file..!");
		return;
	}
	char *str = malloc(100);
	while(fscanf(fp,"%s",str) == 1) {
		if(strcmp(str,word) == 0) {
			int pos = ftell(fp);
			fseek(fp,pos-strlen(str),SEEK_SET);
			fprintf(fp,"%s",replacement);
		}
	}
}

int main( int argc,char *argv[]) {
	if(argc != 4) {
		printf("Invalid Arguments..!");
		return 0;
	}
	replaceWordWithAnother(argv[1],argv[2],argv[3]);
}
