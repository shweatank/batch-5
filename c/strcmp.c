#include<stdio.h>
void main()
{
char s1[50],s2[50];
int i;
printf("Enter s1 and s2:\n");
scanf("%s%s",s1,s2);
for(i=0;s1[i]&&s2[i];i++)
{
if(s1[i]!=s2[i])
break;
}
if(s1[i]==s2[i])
printf("strings are equal\n");
else
printf("strings are not equal\n");
}
