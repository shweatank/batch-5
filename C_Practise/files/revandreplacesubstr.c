#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void reverse(char *substr,char *rev) {
	int i = 0,j = strlen(substr) - 1;
	for(i;i<strlen(substr);i++,j--) {
		rev[i] = substr[j];
	}
	rev[i] = '\0';
}

void revsubstrandreplace(char *argv[]) {
	FILE *fp = fopen(argv[2],"r+");
	if(!fp) {
		printf("File not found\n");
		exit(0);
	}
	char str[100];
	int sublen = strlen(argv[1]);
	int mainlen;
	int index = 0;
	char *p;
	char rev[100];
	while(fgets(str,100,fp)) {
		int flag = 0;
		mainlen = strlen(str);
		p = str;
		while(p = strstr(p,argv[1])) {
			flag = 1;
			index = p - str;
			reverse(argv[1],rev);
			strncpy(p,rev,sublen);
			p += sublen;
		}
		if(flag) {
			fseek(fp,-mainlen,SEEK_CUR);
			fputs(str,fp);
		}
	}
	fclose(fp);
}

int main(int argc,char *argv[]) {
	if(argc != 3) {
		printf("Invalid arguments\n");
		exit(0);
	}
	revsubstrandreplace(argv);
}

