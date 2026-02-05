#include<stdio.h>
void Separate(int );
int main()
{
	int n;
	puts("enter the number");
	scanf("%d",&n);
	Separate(n);
}
void Separate(int n)
{
	int even=0,odd=0,a,cnt=1;
	while(n)
	{
		a=n%10;
		if(a%2==0)
		{
			even=(even*10)+a;
			cnt=cnt*10;
		}
		else
		{
			odd=(odd*10)+a;
		}
		n/=10;
	}
	even=even*cnt;
	even=even+odd;
	printf("%d\n",even);
}
