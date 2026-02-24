#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int my_getline_command(char **lineptr, int *n, FILE *fp) {
	if(fp == NULL) {
		printf("No file found\n");
		exit(0);
	}
again:	if(*n == 0) *n = 1;
	*lineptr = NULL;
	*lineptr = malloc(*n);
	int line_char_count = 0;
	char ch;
	while((ch = fgetc(fp)) != EOF) {
		(*lineptr)[line_char_count] = ch;
		line_char_count++;
		if(line_char_count >= (*n - 1))
			*lineptr = realloc(*lineptr,1);
		if(ch == '\n') {
			(*lineptr)[line_char_count] = '\0';
			printf("Retrived line of length %d: \n%s",line_char_count,*lineptr);
			free(*lineptr);
			goto again;
		}
	}
}

int main(int argc,char *argv[]) {
	char **lineptr;
	int size = 2;
	FILE *fp = fopen(argv[1],"r");
	my_getline_command(lineptr,&size,fp);
}


