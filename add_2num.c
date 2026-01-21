//addition of 2num without using + operator

#include<stdio.h>
int main()
{
	int num1,num2,carry=0;
	printf("enter num1 : ");
	scanf("%d",&num1);
	printf("enter num2 : ");
	scanf("%d",&num2);
	while(num2!=0)
	{
		carry=(num1 & num2)<<1;
		num1=num1^num2;
		num2=carry;
	}

	printf("sum : %d\n",num1);
}
