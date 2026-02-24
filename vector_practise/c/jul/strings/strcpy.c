#include <stdio.h>
void mystrcpy(char * dest,const char *src);
int main() {
	char str[20],str2[20];
	printf("Enter the string upto 20 characters: ");
	scanf("%[^\n]s",str);
	mystrcpy(str2,str);
	printf("Copied string is: %s\n",str2);
}

void mystrcpy(char *dest,const char *src) {
	int i = 0;
	while(src[i]) {
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
}


