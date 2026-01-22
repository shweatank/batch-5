#include<stdio.h>
int Rev(int n);
int main()
{
	int n;
	puts("enter the number");
	scanf("%d",&n);
	int rev=Rev(n);
	printf("reverse num is %d\n",rev);
}
int Rev(int n)
{
	int rev=0;
	while(n)
	{
		rev=(rev*10)+(n%10);
		n/=10;
	}
	return rev;
}
