//string reverse without lopp
#include<stdio.h>
#include<string.h>
void reverse(char *,int);
int half_len;
int main()
{
	char s[20];
	printf("enter the string..\n");
	scanf("%s",s);
	int len=strlen(s)-1;
	half_len=len/2;
	reverse(s,len);
	printf("%s\n",s);
}
void reverse(char *s,int len)
{	
	char temp;
	temp=*s;
	*s=s[len];
	s[len]=temp;
	if(len>0)
	reverse(s+1,len-2);
}
