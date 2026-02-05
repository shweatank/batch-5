#include<stdio.h>
#include<string.h>
void Rev(char *,int);
int main()
{
	char str[100];
	puts("enter the string");
	scanf("%s",str);
	int n=strlen(str)-1;
	Rev(str,n);
	printf("%s\n",str);
}
void Rev(char *str,int n)
{
	static int k=0;
	char ch;
	if(k>n)
	{
		return;
	}
	else
	{
		ch=str[k];
		str[k++]=str[n];
		str[n]=ch;
		Rev(str,n-1);
	}
}
