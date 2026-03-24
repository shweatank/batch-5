#include"header.h"
void str_delete(char*s1,char*s2)
{
int i,j,k;
for(i=0;s1[i];i++)
{
for(j=0;s2[j];j++)
{
if(s1[i]==s2[j])
{
for(k=j;s2[k];k++)
s2[k]=s2[k+1];
j--;
}
}
}
}
void main()
{
char s1[20],s2[20];
printf("enter string 1\n");
scanf("%s",s1);
printf("enter string 2\n");
scanf(" %s",s2);
str_delete(s1,s2);
printf("s1=%s\n",s1);
printf("s2=%s\n",s2);
}
