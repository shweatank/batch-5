#include<stdio.h>
#include<string.h>
void main()
{
	int l,i;
	char s1[30],ch;
	printf("Enter a string");
	//gets(s1);
	scanf("%s",s1);
	l=strlen(s1);
	for(i=0;i<l/2;i++)
	{
		ch =s1[i];
		s1[i]=s1[l-1-i];
		s1[l-1-i]=ch;
	}
	printf("%s",s1);
}

