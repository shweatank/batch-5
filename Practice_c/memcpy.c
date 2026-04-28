#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main()
{
	char s[]="Hello",p[]="Hello",*q;
	memcpy(s+1,s,4);
	memmove(p+1,p,4);
	printf("%s\n",s);
	printf("%s\n",p);
}
