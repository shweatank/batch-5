#include<stdio.h>
int EvenSum(int);
int OddSum(int);
int main()
{
	int n;
	puts("enter the n");
	scanf("%d",&n);
	int evensum=EvenSum(n);
	int oddsum=OddSum(n);
	printf("even sum is %d\n",evensum);
	printf("odd sum is %d\n",oddsum);
	return 0;
}
int EvenSum(int n)
{
	int i=1;
	int sum=0;
	for(;i<=n;i++)
	{
		if(i%2==0)
			sum+=i;
	}
	return sum;

}
int OddSum(int n)
{
	int i=1,sum=0;
	for(;i<=n;i++)
	{
		if(i%2!=0)
			sum+=i;
	}
	return sum;
}
