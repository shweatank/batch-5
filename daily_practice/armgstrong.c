#include<stdio.h>
#include<math.h>
int armstrong(int a)
{
	int cnt=0,temp=a,sum=0;
	do
	{
		cnt++;
		temp/=10;
	}while(temp);
	temp=a;
	while(temp)
	{
		sum+=pow(temp%10,cnt);
		temp/=10;
	}
	if(sum==a)
		return 1;
	else
		return 0;
		
}
int main()
{
	int n;
	puts("enter the number");
	scanf("%d",&n);
	if(armstrong(n))
		puts("it is armgstrong");
	else
		puts("no an armgstrong");
}
