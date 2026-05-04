#include<stdio.h>
void main()
{
int num,i;
printf("enter the num\n");
scanf("%d",&num);
int min=9,s_min;
for( ;num;num/=10)
{
int r=num%10;
if(r<min)
{
s_min=min;
min=r;
}
}

printf("min=%d smin=%d\n",min,s_min);
}
