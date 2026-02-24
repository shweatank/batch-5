#include <stdio.h>
int mystrlen(char *);
int main() {
	char str[20];
	printf("Enter the string: ");
	scanf("%[^\n]s",str);
	int len = mystrlen(str);
	printf("The lenght of %s is %d\n",str,len);
}

int mystrlen(char *str) {
	int i = 0;
	while(str[i]) i++;
	return i;
}
