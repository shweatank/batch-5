//wap to pass the function pointer as argument to the another function 

#include<stdio.h>

int(*res)(int,int,char);

int calculte(int x,int y,char op)
{
	

}

void print_result(res)
{

}

int main()
{
	int num1=10,num=20;
	char ch;
	printf("enter operator : ");
	scanf("%c",&ch);
	print_result(calculate(num1,num2,ch));
}
