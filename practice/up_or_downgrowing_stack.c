#include<stdio.h>
void fun(int *p)
{
	int b;
	printf("b addr:%p	a addr:%p\n",&b,p);
	if(&b > p)
		printf("up growing\n");
	else
		printf("down growing\n");
}
int main()
{
	int a;
	fun(&a);
	return 0;
}
