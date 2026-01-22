#include<stdio.h>
int main()
{
	int n,m;
	puts("enter the number");
	scanf("%d",&n);
	puts("enter power");
	scanf("%d",&m);
	long num=1;
	for(int i=1;i<=m;i++)
		num=num*n;
	printf("%ld",num);
}
