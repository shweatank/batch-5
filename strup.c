#include<stdio.h>
#include<string.h>
void main()
{
	int i;
	char s1[30];
	printf("Enter string:");
	gets(s1);
	for (i=0;s1[i]!='\0';i++)
	{
		if(s1[i]>='a'&&s1[i]<='b')
		{
			s1[i]=s1[i]+32;
		}
	}
	printf("%s",s1);
}

