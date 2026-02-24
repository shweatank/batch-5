#include <stdio.h>
#include <string.h>
void mystrcat(char *dest,char *src);
int main() {
	char s1[20] = "Vector";
	char s2[20] = "India";
	mystrcat(s1,s2);
	printf("%s\n",s1);
}

void mystrcat(char *dest,char *src) {
	int len = strlen(dest),i = 0;
	while(src[i]) {
		dest[len] = src[i];
		i++;
		len++;
	}
	dest[len] = '\0';
}
