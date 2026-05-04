#include<stdio.h>
int sum(int *);
void main()
{
int *p=0,a[5]={1,2,3,4,5};
int r=sum(a);
printf("sum=%d\n",r);
*p=10;
}
int sum(int * a)
{
int i,sum=0;
for(i=0;i<5;i++)
sum=sum+a[i];
return sum;
}


