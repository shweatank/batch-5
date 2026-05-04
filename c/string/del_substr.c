#include<stdio.h>
#include<string.h>
void main()
{
char m[20],s[10],*p,*q;
printf("enter main str\n");
scanf("%s",m);
printf("entre sub str\n");
scanf("%s",s);
p=m;
while(q=strstr(p,s))
strcpy(q,q+strlen(s));

printf("%s\n",m);
}
