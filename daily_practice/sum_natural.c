#include<stdio.h>
int main()
{
	int n;
	printf("enter the n\n");
	scanf("%d",&n);
	int i,sum=0;
	for(i=1;i<=n;i++)
		sum+=i;
	printf("sum is %d\n",sum);
}
