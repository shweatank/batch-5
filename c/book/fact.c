#include<stdio.h>
void main()
{
int num;
printf("enter the num\n");
scanf("%d",&num);
int fact=1;
while(num>=1)
{
fact=fact*num;
num--;
}
printf("fact=%d\n",fact);
}

