#include<stdio.h>
int main()
{
	int num,cnt=0;
	printf("enter number : ");
	scanf("%d",&num);
	for(int i=31;i>=0;i--)
	{
		if((num>>i)&1)
			cnt++;
	}
	printf("set bit count : %d\n",cnt);
}
