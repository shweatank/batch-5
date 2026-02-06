#include<stdio.h>
int add(int a,int b);
int sub(int a,int b);
int mul(int a,int b);
int div(int a,int b);
int main()
{
	int a,b;
	printf("enter the a and b\n");
	scanf("%d%d",&a,&b);
	int (*ptr)(int,int);
	char ch;
	puts("enter the operation");
	scanf(" %c",&ch);
	switch(ch)
	{
		case'+':ptr=add;
			break;
		case'-':ptr=sub;
			break;
		case'*':ptr=mul;
			break;
		case'/':ptr=div;
	}
	int res=ptr(a,b);
	printf("%d\n",res);
}
int add(int a,int b)
{
	return a+b;
}
int sub(int a,int b)
{
	return a-b;
}
int mul(int a,int b)
{
	return a*b;
}
int div(int a,int b)
{
	return a/b;
}
