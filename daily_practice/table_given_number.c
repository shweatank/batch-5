#include<stdio.h>
void Table(int);
int main()
{
	int n;
	puts("enter the number");
	scanf("%d",&n);
	Table(n);
	return 0;
}
void Table(int n)
{
	int i=1;
	for(;i<=10;i++)
	{
		printf("%*d\n",2,n*i);
	}
}
