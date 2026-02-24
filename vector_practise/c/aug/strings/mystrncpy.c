#include <stdio.h>
void mystrncpy(char *dest,const char *src,int size);
int main() {
	int size,srcsize,destsize;
	printf("Enter the source and destination string size: ");
	scanf("%d %d",&srcsize,&destsize);
	printf("Enter the no of characters to be copied: ");
        scanf("%d",&size);
	if(size > destsize) {
		printf("The destination size is less the given size");
		return 0;
	}	
	char src[srcsize],dest[destsize];
	printf("Enter the source string: ");
	scanf("%s",src);
	if(strlen(src) < size) {
		printf("The source string length is less than size");
		return 0;
	}
	mystrncpy(dest,src,size);
	printf("%s\n",dest);
}

void mystrncpy(char *dest,const char *src,int size) {
	int i = 0;
    	while (i < size && src[i] != '\0') {
        	dest[i] = src[i];
        	i++;
    	}
        dest[i] = '\0';
}
