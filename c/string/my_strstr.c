#include<stdio.h>
char * my_strstr(const char *, int);
void main()
{
char s[10];
char ch;
printf("enter the string\n");
scanf("%s",s);
printf("enter the char\n");
scanf(" %c",&ch);
char *p;
p=my_strstr(s,ch);
if(*p!=0)
printf("char is present\n");
else
printf("char is not present\n");
}

char * my_strstr( const char * s, int ch)
{
int i;
for(i=0;s[i];i++)
{
if(s[i]==ch)
return &s[i];
}
return 0;
}
