#include<stdio.h>
int (*ptr) (int,int);
int add(int,int);
int sub(int,int);
void main()
{
int num1,num2;
printf("enter the numbers\n");
scanf("%d%d",&num1,&num2);
ptr=add;
int (*ptr) (int,int)=add;
int sum_r=ptr(num1,num2);
printf("sum_r=%d\n",sum_r);
}

int add(int a,int b)
{
return a+b;
}

int sub(int a,int b)
{
return a-b;
}
