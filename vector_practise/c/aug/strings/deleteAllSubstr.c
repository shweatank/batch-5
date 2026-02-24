#include <stdio.h>
#include <string.h>
int main() {
	char str[20];
	printf("Enter the string upto 20 characters: ");
	scanf("%s",str);
	char sub[20];
	printf("Enter the substring: ");
	scanf("%s",sub);
	char *p;
	long int len = strlen(sub);
	while(p = strstr(str,sub)) {
	       memmove(p,p+len,strlen(p+len)+1);
	}
	printf("%s",str);
}

