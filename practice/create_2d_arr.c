#include<stdio.h>
#include<stdlib.h>
int main()
{
	//a[2][5]
	//array of ptrs
	int *a[2];
	for(int i=0;i<2;i++)
	{
		a[i]=calloc(5,sizeof(int));
	}
	for(int i=0;i<2;i++)
	{
		for(int j=0;j<5;j++)
		{
			a[i][j]=i+j;
		}
	}
	//ptr to an array
	int (*ptr)[5];
	ptr=calloc(2,5*sizeof(int));
	for(int i=0;i<2;i++)
	{
		for(int j=0;j<5;j++)
		{
			ptr[i][j]=i+j+10;
		}
	}

	for(int i=0;i<2;i++)
	{
		for(int j=0;j<5;j++)
		{
			printf("%d ",ptr[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	for(int i=0;i<2;i++)
	{
		for(int j=0;j<5;j++)
		{
			printf("%d ",a[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	return 0;
}
