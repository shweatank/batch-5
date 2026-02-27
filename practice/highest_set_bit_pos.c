#include<stdio.h>
int main()
{
	int n,pos=0;
	scanf("%d",&n);

	for(int i=31;i>=0;i--)
		printf("%d ",(n>>i)&1);
	while(n)
	{
		n>>=1;
		pos++;
	}
	printf("highest:%d\n",pos-1);
}
