#include <stdio.h>
#include <string.h>
const char * mystrstr(const char *str,const char *substr) {
	int i = 0,j = 0;
	for(;i<strlen(str);) {
		if(j == strlen(substr)) break;	
		if(str[i] == substr[j]) {
			i++;
			j++;
		}
		else {
			if(j != 0) j = 0;
			else i++;
		}
	}
	if(j == strlen(substr)) return (str + (i-j));
	return NULL;
}

int main() {
	char str[100];
	char substr[100];
	scanf("%[^\n]s",str);
	scanf("%s",substr);
	const char *p = mystrstr(str,substr);
	(p != NULL) ? printf("Substr found at %ld index",p - str) : printf("Substr not found");
}
