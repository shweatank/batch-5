#include"header.h"
void reverse(char*p)
{
char *q=p+strlen(p)-1,t;
while(p<q)
{
t=*p;
*p=*q;
*q=t;
p++;
q--;
}
}
void main()
{
char s[10];
printf("enter the string:");
scanf("%s",s);
char *p="hi";
p[0]='8';
int i;
reverse(s);
printf("%s\n",s);
}
