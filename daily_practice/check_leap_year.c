#include<stdio.h>
int main()
{
	int n;
	printf("enter the year\n");
	scanf("%d",&n);
	if(n%400==0)
		puts("leap year");
	else if(n%4==0 && n%100 !=0)
		puts("leap year");
	else
		puts("not a leap year");
	return 0;
}
