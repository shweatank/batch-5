#include<stdio.h>
void main()
{
int num,i;
printf("enter the num\n");
scanf("%d",&num);

for(i=2;i<num;i++)
{
if(num%i==0)
break;
}
if(i==num)
printf("prime\n");
else
printf("not prime\n");
}
