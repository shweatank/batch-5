#include<stdio.h>
int Check(int);
int main()
{
	int n;
	puts("enter the number");
	scanf("%d",&n);
	Check(n);
}

int Check(int n)
{
	if(n<0)
		puts("given number is negative");
	else if(n==0)
		puts("given number is zero");
	else
		puts("given number is positive");

}
