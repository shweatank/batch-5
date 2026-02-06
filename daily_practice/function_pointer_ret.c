#include<stdio.h>
int sub(int ,int );
int(*add(int,int))(int ,int );
int main()
{
	int a,b;
	puts("enter the a and b");
	scanf("%d%d",&a,&b);
	int(*(*ptr)(int,int))(int,int);
	ptr=add;
	int res=ptr(a,b)(a,b);
	printf("%d\n",res);
}
int(*add(int a,int b))(int ,int )
{

	return sub;
}
int sub(int a,int b)
{
	return a-b;
}
