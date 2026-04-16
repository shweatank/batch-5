#include<stdio.h>
void ownstrcpy(char *,char *);
void main()
{
	char s[20],d[20];
	printf("enter the source string....\n");
	scanf("%s",s);
	ownstrcpy(d,s);
	printf("copied string:%s\n",d);
}
void ownstrcpy(char *d,char *s)
{
	while((*d++=*s++));
}

