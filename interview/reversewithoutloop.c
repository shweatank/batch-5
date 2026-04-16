//string reverse without lopp
#include<stdio.h>
#include<string.h>
void reverse(char *,char *);
int main()
{
	char s[20];
	printf("enter the string..\n");
	scanf("%s",s);
	char *q=s+strlen(s)-1;;
	reverse(s,q);
	printf("%s\n",s);
}
void reverse(char *s,char *q)
{	

	char temp;
	temp=*s;
	*s=*q;
	*q=temp;
	s++;
	q--;
	if(s<q)
	reverse(s,q);
}
