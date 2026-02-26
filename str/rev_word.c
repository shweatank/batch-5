#include<stdio.h>
void main()
{
char s[20],i,j=0,len;
printf("enter the str\n");
scanf("%s", s);
for(len=0;s[len]; len++);

for(i=0;i<=len;i++)
{
if(s[i]==' ' || s[i]=='\0')
{
int temp=i;
for(i--; j<i; j++,i--)
{
char t;
t=s[j];
s[j]=s[i];
s[i]=t;
}
printf("i=%d j=%d \n",i,j);
i=temp;
j=i+1;
printf("i=%d j=%d \n",i,j);

}
}

printf("%s\n",s);
}
