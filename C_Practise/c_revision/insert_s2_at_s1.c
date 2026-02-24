#include <string.h>
#include <stdio.h>
#include "getstring.c"

char *insertInString1(char *s2,char *s1,int n) {
	int s1len = strlen(s1);
	int s2len = strlen(s2);
	int flag = 0,i;
	char *res = malloc(s1len+s2len+1);
	for(i = 0;i<(s1len+s2len);i++) {
		if(i == n) flag = 1;
		if(i == n+s2len-1) flag = 0;
		res[i] = (flag) ? s2[i] : s1[i];
	}
	res[i] = '\0';
	return res;
}

int main() {
	char *s1 = getstring();
	char *s2 = getstring();
	int n;
	scanf("%d",&n);
	printf("%s\n",insertInString1(s2,s1,n));
}
