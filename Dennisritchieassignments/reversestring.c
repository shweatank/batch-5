#include<stdio.h>
#include<string.h>
void main()
{
	char s[30];
	printf("enter the string..\n");
	scanf("%s",s);
	int i,j,temp;
	for(i=0,j=strlen(s)-1;i<j;i++,j--)
	{
		temp=s[i];
		s[i]=s[j];
		s[j]=temp;
	}
	printf("%s",s);
}
