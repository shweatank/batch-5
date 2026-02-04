#include<stdio.h>
int main()
{
	int n;
	puts("enter the n");
	scanf("%d",&n);
	int i=0;
	int cnt=0;
	for(;;i++)
	{
		int x=i;
		cnt=0;
		while(x)
		{
			x=x&(x-1);
			cnt++;
		}
		if(cnt==2)
			n--;
		if(n==0)
		{
			printf("%d\n",i);
			break;
		}
	}
}
