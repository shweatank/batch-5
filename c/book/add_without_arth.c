#include<stdio.h>
void main()
{
int num1,num2;
printf("enter the num1\n");
scanf("%d",&num1);
printf("enter the num2\n");
scanf("%d",&num2);
while(num2)
{
num1++;
num2--;
}
printf("sum=%d\n",num1);
}
