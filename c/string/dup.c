#include<stdio.h>
void main()
{
char s[20];
int i,j,k;
printf("enter the string\n");
scanf("%s",s);
int slen;
for(i=0; s[i]; i++);
 slen=i;
printf("slen=%d\n",i);

for(i=0;i<slen;i++)
{
for(j=i+1;j<slen; j++)
if(s[i]==s[j])
{
for(k=j; k<slen ; k++)
s[k]=s[k+1];
j--;
slen--;
}
}

printf("%s\n",s);

}
