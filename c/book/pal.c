#include<stdio.h>
void main()
{
int num,rev=0,r;
printf("enter num\n");
scanf("%d",&num);
int temp=num;
for( ;num;num/=10)
{
r=num%10;
rev=rev*10+r;
}

if(rev==temp)
printf("pal\n");
else
printf("not pal\n");
}

