#include <stdio.h>
#include <string.h>
void reverseSub(const char *sub,char * rev);
void replaceSubstrWithRev(char *str,char *sub,char *rev);
int main() {
	char str[100],sub[100],rev[100];
	printf("Enter the string: ");
	scanf("%s",str);
	printf("Enter the sub string: ");
	scanf("%s",str);
	reverseSub(sub,rev);
	replaceSubstrWithRev(str,sub,rev);
	printf("%s\n",str);
}

void reverseSub(const char *sub,char * rev) {
	int len = strlen(sub);
	int i,j;
	for(i = 0,j = len-1;i<len;i++,j--) {
		rev[i] = sub[j];
	}
	//rev[i] = '\0';
}

void replaceSubstrWithRev(char *str,char *sub,char *rev) {
	char *p;
	long int len = strlen(rev);
	while(p = strstr(str,sub)) {
		memmove(p,rev,len);
		p += len;
	}
}
