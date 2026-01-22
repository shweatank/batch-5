#include<stdio.h>
int main()
{
	int n;
	puts("enter the number");
	scanf("%d",&n);
	int cnt=0;
	do
	{
		cnt++;
		n/=10;
	}while(n);
	printf("no of digits is %d",cnt);
	return 0;
}
