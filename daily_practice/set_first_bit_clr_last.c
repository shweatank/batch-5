//setting the right most bit and clear the left most set bit  after check for prime and print for a given number

#include<stdio.h>
#include<math.h>

int isprime(int n)
{
	int s=sqrt(n);
	if(n<2)
		return 0;
	if(n==2)
		return 1;
	for(int i=2;i<=s;i++)
	{
		if(n%i==0)
			return 0;
	}
	return 1;
}
int main()
{
	int n;
	puts("enter the n");
	scanf("%d",&n);
	int i=0;
	for(i=0;i<32;i++)
	{
		if(((n>>i)&1)==0)
		{
			n=n|(1<<0);
			break;
		}
	}
	for(i=31;i>=0;i--)
	{
		if(((n>>i)&1)==1)
		{
			n=n&(~(1<<i));
			break;
		}
	}
	if(isprime(n))
		printf("%d is prime number\n",n);
	else 
		printf("it is not prime number\n");
}
