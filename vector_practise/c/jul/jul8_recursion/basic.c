#include<stdio.h>
void f1(int);
int main()
{
	int a = 10;
	f1(a);
}
void f1(int a)
{
	printf("a=%d\n",a);
	if(++a<15)
		f1(a);
}
