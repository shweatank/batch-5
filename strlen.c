#include<stdio.h>
#include<string.h>
void main()
{
	char name[30];
	int count=0;
	printf("Enter name:");
	gets(name);
	count=strlen(name);
	puts(name);
	printf("length of string is :%d",count);
}

