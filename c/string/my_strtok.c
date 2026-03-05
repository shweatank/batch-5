#include<stdio.h>
char * my_strtok(char *, const char *);
void main()
{
char s[10],ch,*p;
printf("enter the string\n");
scanf("%s",s);
printf("enter the ch\n");
scanf(" %c",&ch);
p=my_strtok(
