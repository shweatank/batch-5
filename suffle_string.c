#include<string.h>
#include<stdio.h>
int main()
{
	char s1[10],s2[10];


	printf("enter the strings\n");
	scanf("%s%s",s1,s2);

	unsigned char ch;

	int len=strlen(s2);

	for(int j=0;j<len;j++)
		ch^=s1[j]^=s2[j];

	printf("diffrent char=%c\n",ch);
}

