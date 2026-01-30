#include<stdio.h>
//const static int x=10;
int main()
{
	const static int x=10;

//	x=20;

	int *ptr;
	//int x=10,y=20;
	ptr=&x;
	//ptr=&y;
	*ptr=20;
	printf("%d",*ptr);
}
