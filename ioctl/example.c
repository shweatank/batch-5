#include<stdio.h>
int main()
{
	char ch[]={'a','b','c','d'};
	char *p=ch;
	*p++;
	printf("%c %c",*++p,*p++);
}
