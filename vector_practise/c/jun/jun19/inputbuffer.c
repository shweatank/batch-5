#include <stdio.h>
#include <stdio_ext.h>
int main() {
	char ch;
	do {
		printf("Enter the character(# to stop): ");
		scanf("%c",&ch);
		while(getchar() != '\n');
	//	__fpurge(stdin);
		printf("%c\n",ch);
	}while( ch != '#');
	return 0;
}
