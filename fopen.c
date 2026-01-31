#include <stdio.h>
#include <string.h>

int main() {
	FILE *fp;
	char str[20],next[20];
	while(1) {
		fp = fopen("data.txt","r+");
		char *ret = fgets(str,20,fp);
		if(ret != NULL) {
			printf("%s",str);
			ret = fgets(str,20,fp);
			fseek(fp,-strlen(str),SEEK_CUR);
			fgets(next,20,fp);
			if(strcmp(str,next)) printf("%s",next);
		}
	}
}	
