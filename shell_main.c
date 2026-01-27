#include<stdio.h>
int add(int a,int b)
{
	return a+b;
}
int sub(int a,int b)
{
	return a-b;
}
int main()
{
	printf("in main..\n");
	int r=add(2,3);
	printf("%d\n",r);
	int y=sub(9,7);
	printf("%d\n",y);
	return 0;
}
