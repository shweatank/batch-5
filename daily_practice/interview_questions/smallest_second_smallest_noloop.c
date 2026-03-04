#include<stdio.h>
int Find_small(int n)
{
	if(n==0)
		return 0;
	static int small=10,sec_small=10;
	if((n%10)<small)
	{
		sec_small=small;
		small=(n%10);
	}
	if((n%10)>small && (n%10)<sec_small)
		sec_small=(n%10);
	Find_small(n/=10);
	printf("small is %d\nsecond small is %d\n",small,sec_small);

}
int main()
{
	int n;
	printf("enter the number\n");
	scanf("%d",&n);
	Find_small(n);
}
