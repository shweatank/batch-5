#include<stdio.h>
int my_strcmp(const char *, const char *);
void main()
{
char s1[10],s2[10],r;
printf("enter the s1 string\n");
scanf("%s",s1);
printf("enter the s2 string\n");
scanf("%s",s2);
r=my_strcmp(s1,s2);
if(r==0)
printf("strings s1 and s2 are equal\n");
else if(r<0)
printf("string s1 is smaller than s2\n");
else if(r>0)
printf("string s1 is greater than s2\n");
}

int my_strcmp(const char *s1, const char *s2)
{
int i;
for(i=0; s1[i]; i++)
{
if(s1[i]!=s2[i])
break;
}
if(s1[i]==s2[i])
return 0;
else if(s1[i]<s2[i])
return -1;
else
return 1;
}
