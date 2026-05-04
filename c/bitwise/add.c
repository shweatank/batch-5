#include<stdio.h>
void main()
{
unsigned int n1,n2,sum;
printf("enter n1\n");
scanf("%d",&n1);

printf("enter n2\n");
scanf("%d",&n2);

if(n1==n2)
sum=n1<<1;
else
{
n2=n1^n2;
sum=n1<<1|n2;
}
printf("sum=%d\n",sum);
}

