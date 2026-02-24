//change the case of the given character using goto labels
#include <stdio.h>
#include <ctype.h>
#include<stdio_ext.h>
void main() {
	char ch;
begin:  printf("Enter the character: ");
// 	fflush(stdout);
	__fpurge(stdin);
	scanf("%c",&ch);
	if(isupper(ch)) {
		printf("The lower case of given character is: %c\n",ch+32);
	}
	else if(islower(ch)) {
		printf("The uppercase of given character is: %c\n",ch-32);
	}
	else {
		printf("invalid input\n");
		goto begin;
	}
}
