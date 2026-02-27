#include<stdio.h>
void swap(int *a,int *b)
{
	*a=*a+*b;
	*b=*a-*b;
	*a=*a-*b;
	//*a^=*b^=*a^=*b;
}
int main()
{
	int a=7,b=9;
	swap(&a,&b);
	printf("%d %d\n",a,b);
}
