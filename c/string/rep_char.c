#include<stdio.h>
void main()
{
char s[20];
printf("enter the str\n");
scanf("%s",s);
int k;
int i,j,c=0;
for(i=0;i<s[i];i++,c=0)
{
for(j=i+1;j<s[j];j++)
{
if(s[i]==s[j])
{
for(k=j;s[k];k++)
s[k]=s[k+1];
j--;
c++;
}
}
if(c>=1)
printf(" %c",s[i]);
}
printf("\n");
}
