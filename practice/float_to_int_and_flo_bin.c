#include<stdio.h>
union u
{
	float f;
	int n;
}var;
int main()
{
	var.f=3.4;
	printf("f to i of float var:f is:%d\n",var.n);
	for(int i=31;i>=0;i--)
	{
		printf("%d ",((var.n)>>i)&1);
	}
	return 0;

}
