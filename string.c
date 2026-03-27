#include<stdio.h>
void modify(char s[])
{
	s[3]='o';
}
void main()
{
	char *s[]="gurunath";

	modify(s);
	printf("%s",s);
}
