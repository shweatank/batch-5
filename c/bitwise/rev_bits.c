#include<stdio.h>
void main()
{
unsigned int num;
int i,j,m,n;
printf("enter the num\n");
scanf("%d",&num);
int pos=31;
for(pos=31;pos>=0;pos--)
printf("%d ",num>>pos&1);
printf("\n");
for(i=0,j=31; i<j; i++,j--)
{
m=num>>i&1;
n=num>>j&1;
if(m!=n)
{
num=num^(1<<i);
num=num^(1<<j);
}
}
for(pos=31;pos>=0;pos--)
printf("%d ",num>>pos&1);
printf("\n");
}

