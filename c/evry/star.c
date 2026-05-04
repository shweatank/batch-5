#include<stdio.h>
void main()
{
char s[20];
printf("enter the str\n");
scanf("%[^\n]",s);
int i;
for(i=0;s[i];i++)
{
if(s[i]==' ')
s[i]='*';
}
printf("%s\n",s);
}
