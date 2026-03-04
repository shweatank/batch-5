#include<stdio.h>

int SetBit(int n)
{
	int cnt=0;
	while(n)
	{
		n=n&(n-1);
		cnt++;
	}
	return cnt;
}
int main()
{
	int n;
	printf("enter the size\n");
	scanf("%d",&n);
	int res=SetBit(n);
	printf("set bit count of %d number is %d\n",n,res);
}
