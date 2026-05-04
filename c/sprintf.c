#include<stdio.h>
void main()
{
int a=10;
char ch='a';
char s[10];

sprintf(s,"%d %c",a,ch);
printf("%s\n",s);
}
