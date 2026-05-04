#include<stdio.h>
void main()
{
int num;
printf("enter num\n");
scanf("%d",&num);
int i,sum=0;
for(i=1;i<num;i++)
{
//int sum=0;
if(num%i==0)
sum=sum+i;
}
if(sum==num)
printf("perfect\n");
else("not\n");
}
