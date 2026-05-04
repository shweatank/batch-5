#include<stdio.h>
int main()
{
char s[50],i,j,slen;
scanf("%[^\n]",s);

for(slen=0;s[slen];slen++);

for(i=0;i<slen; i++)
{
if(s[i]==' ' && s[i+1]==' ')
{
for(j=i;j<slen;j++)
s[j]=s[j+1];
i--;
}
}

printf("%s\n",s);
}

