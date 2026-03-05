#include<stdio.h>
#include<string.h>
void main()
{
char s[10],*p;
printf("enter the string\n");
scanf("%s",s);

p=strtok(s," ");

printf("%s",p);

}
