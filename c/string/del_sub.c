#include<stdio.h>
#include<string.h>
void main()
{
char m[20],s[10],*p,*q;
printf("enter the main str\n");
scanf("%s",m);
printf("enter the sub str\n");
scanf("%s",s);
p=m;

while(q=strstr(p,s))
strcpy(q,q+strlen(s));

printf("%s\n",m);
}
