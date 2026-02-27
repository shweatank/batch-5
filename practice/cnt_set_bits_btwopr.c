#include<stdio.h>
int main()
{
	int n,cnt=0;
	scanf("%d",&n);
	while(n)
	{
		n=n&(n-1);
		cnt++;
	}
	printf("no.of set bits:%d\n",cnt);
	return 0;
}
