#include<stdio.h>
const static int x=20;

int main()
{
	/*
	const int x=10;
	printf("num : %d\n",x);
	int *ptr;
	ptr=&x;
	*ptr=100;
	printf("num : %d\n",x);
	*/

	/*
	const int x=10;
//	x=20;   leads to error
	int *ptr=&x;
	*ptr=20;	//acceptable	
	printf("num : %d\n",x);

	*/
/*
	int const x=10;
	printf("num : %d\n",*x);
//	x=20;	leads to error
	printf("num : %d\n",x);

	int const ptr=20;
	printf("num : %d\n",ptr);
	*/
/*
	int x=10,y=30;
	int *const ptr=&x;
	*ptr=20;	//valid ptr const
	ptr=&y;		//error read only
	printf("%d\n",*ptr);
*/
/*
	//x=30; //error
	int *ptr=&x;
	*ptr=30;	//segmentation fault
	printf("num : %d\n",x);

	*/
}
