#include<stdio.h>
int main()
{
	int num,rev=0,t;
	scanf("%d",&num);
	t=num;
	while(t)
	{
		rev=(rev*10)+t%10;
		t/=10;
	}
	if(rev==num)
	{
		printf("palindrome");
	}
	else
	{
		printf("not a palindrome");
	}
}

