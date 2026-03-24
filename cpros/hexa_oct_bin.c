#include"header.h"
void reverse(char*s)
{
char *d=s+strlen(s)-1,t;
while(s<d)
{
t=*s;
*s=*d;
*d=t;
s++;
d--;
}
}
void itob(int n,char*str,int base)
{
int r,i=0;
if(base==2)
{
while(n)
{
r=n%base;
str[i++]=r+'0';
n=n/base;
}
str[i]='\0';
reverse(str);
printf("%s\n",str);
}
else if(base==8)
{
while(n)
{
r=n%base;

}
}
else if(base==16)
{



}

}
void main()
{
char s[10];
int n,base;
printf("enter number\n");
scanf("%d",&n);
printf("enter the base 2 8 16\n");
scanf("%d",&base);
itob(n,s,base);
}
