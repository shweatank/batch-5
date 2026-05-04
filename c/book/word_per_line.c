#include<stdio.h>
int main()
{
char s[20],i,j,slen,k;
scanf("%[^\n]",s);

for(slen=0;s[slen];slen++);

for(i=0;i<slen;i++)
{
if(s[i]!=' ')
printf("%c",s[i]);
else
{
if(s[i+1]!=' ')
printf("\n");
else
{
for(j=i;j<slen;j++)
s[j]=s[j+1];
i--;
}
}
}
printf("\n");
return 0;
}
