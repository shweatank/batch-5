#include<stdio.h>
void main()
{
int num;
printf("enter num\n");
scanf("%d",&num);
int i;
for(i=2;i<num;i++)
{
if(num%i==0)
break;
}
if(num==i)
printf("prime\n");
else
printf("not\n");
}
