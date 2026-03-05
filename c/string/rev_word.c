#include<stdio.h>
void main()
{
char s[20],i,j=0,temp;
printf("enter the string\n");
scanf("%[^\n]",s);
int len;
for(len=0;s[len]; len++);

for(i=0; i<=len; i++)
{
if(s[i]==' ' || s[i]=='\0')
{
int t=i;
for(i--; j<i; j++,i--)
{
temp=s[i];
s[i]=s[j];
s[j]=temp;
}
j=t+1;
i=t;
}
}

printf("%s\n",s);
}
