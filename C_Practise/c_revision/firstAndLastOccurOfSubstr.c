#include <stdio.h>
#include <string.h>

char * fun(char *str,char *sub) {
	char *ptr = strstr(str,sub);
	if(ptr == NULL) {
		printf("No substring found\n");
		return NULL;
	}
	int start = ptr-str;
	for(int i = start,j = start+strlen(sub)-1;i<j;i++,j--) {
		if(str[i] != str[j]) {
			char temp = str[i];
			str[i] = str[j];
			str[j] = temp;
		}
	}
	while(strstr(ptr,sub) != NULL) {
		ptr = strstr(ptr,sub);
		ptr+=strlen(sub);
	}
	ptr -= strlen(sub);
	memset(ptr,'*',strlen(sub));
	return str;
}

int main() {
	char str[] = "Hello Hello Hello Hello Hello";
	char sub[] = "llo";
	printf("%s\n",fun(str,sub));
}
