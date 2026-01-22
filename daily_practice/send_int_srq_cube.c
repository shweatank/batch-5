#include<stdio.h>
long square(int);
long cube(int);

int main()
{
	int a;
	printf("enter the number\n");
	scanf("%d",&a);
	long num=square(a);
	long num2=cube(a);
	printf("square is %ld\n",num);
	printf("cube is %ld\n",num2);
}
long square(int a)
{
	return a*a;
}
long cube(int a)
{
	return a*a*a;
}
