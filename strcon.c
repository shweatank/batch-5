#include<stdio.h>
#include<string.h>
void main()
{
	char s1[30],s2[30];
	printf("Enter s1");
	scanf("%s",s1);
	printf("Enter s2");
	scanf("%s",s2);
	strcat(s1,s2);
	printf("String after concatenation is :%s",s1);
}

