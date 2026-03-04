#include<stdio.h>
int main()
{
	int n,f=0;
	printf("enter the number\n");
	scanf("%d",&n);
	int small=10,secsmall=10;
	while(n)
	{
		if((n%10)<small)
		{
			secsmall=small;
			small=n%10;
		}
		if((n%10)>small && (n%10)<secsmall)
		{
			secsmall=n%10;
		}
		n/=10;
	}
	if(secsmall==10)
		printf("smallest is %d\n no second smallest\n",small);
	else
		printf("smallest number is %d\n second smallest number is %d\n",small,secsmall);
}
