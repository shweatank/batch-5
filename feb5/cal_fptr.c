#include<stdio.h>
int sum(int x,int y)
{
	return x+y;
}
int mul(int x,int y)
{
	return x*y;
}
int sub(int x,int y)
{
	return x-y;
}
int div(int x,int y)
{
	return x/y;
}
int main()
{
	int choice;
	int(*fptr[4])(int,int)={sum,sub,mul,div}; 
	printf("enter choice : 0.add 1.sub 2.mul 3.div\n");
	scanf("%d",&choice);
	printf("result : %d\n",fptr[choice](10,20));
}
