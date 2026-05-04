#include<stdio.h>
char * my_strcpy(char *,const char *);
void main()
{
char s[10],d[10];
printf("enter the str\n");
scanf("%s",s);
char *p=my_strcpy(d,s);
printf("%s\n",p);
}

char * my_strcpy(char * d,const char *s)
{
char *p=d;
while(*d++=*s++);
return p;
}

