#include<stdio.h>
#include<stdlib.h>
int main()
{
	//a[3][4]
	//array of ptrs
	int *a[3];
	for(int i=0;i<3;i++)
	{
		a[i]=calloc(4,sizeof(int));
	}
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<4;j++)
		{
			a[i][j]=i+j;
		}
	}
	//ptr to an array
	int (*ptr)[4];
	ptr=calloc(3,4*sizeof(int));
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<4;j++)
		{
			ptr[i][j]=i+j+10;
		}
	}
	//double ptr
	int **dptr=NULL;
	dptr=(int**)calloc(3,sizeof(int*));
	for(int i=0;i<3;i++)
	{
		dptr[i]=calloc(4,sizeof(int));
		for(int j=0;j<4;j++)
		{
			dptr[i][j]=i+j+30;
		}
	}
	
		
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<4;j++)
		{
			printf("%d ",dptr[i][j]);
		}
		free(dptr[i]);
		dptr[i]=NULL;
		printf("\n");
	}
	free(dptr);dptr=NULL;
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<4;j++)
		{
			printf("%d ",ptr[i][j]);
		}
		printf("\n");
	}
	free(ptr);ptr=NULL;
	printf("\n");
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<4;j++)
		{
			printf("%d ",a[i][j]);
		}
		free(a[i]);a[i]=NULL;
		printf("\n");
	}
	printf("\n");
	
	return 0;
}
