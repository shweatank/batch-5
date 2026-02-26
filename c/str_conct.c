#include<stdio.h>
void main()
{
char s[100],d[100];
int len,i,j;
printf("Enter the source string:\n");
scanf("%s",s);
printf("Enter the destinetion string:\n");
scanf("%s",d);
for(len=0;s[len];len++);
s[len]=' ';
for(i=0,j=len+1;d[i];i++,j++)
s[j]=d[i];
s[j]=d[i];

printf("%s\n",s);
}
