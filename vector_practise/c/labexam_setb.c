#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void removeFirstAndLastSubstrOccur(int argc,char *argv[]) {
	int sublen = strlen(argv[2]);
	FILE *fp = fopen(argv[1],"r+");
	char str[100];
	while(fgets(str,100,fp)) {
		char *p = str;
		int flag = 0;
		if(p = strstr(p,argv[2])) {
			memmove(p,p+sublen,strlen(p+sublen));
			p+=sublen;
		}
		char *q;
		while(q = strstr(p,argv[2])) {
			p = q;
			p += sublen;
			flag = 1;
		}
		if(flag) memmove(p,p+sublen,strlen(p+sublen));
		fseek(fp,-strlen(str),SEEK_CUR);
		fputs(p,fp);
	}
	fclose(fp);
}

int main(int argc,char *argv[]) {
	removeFirstAndLastSubstrOccur(argc,argv);
}



