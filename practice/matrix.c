#include<stdio.h>
#include<stdlib.h>
int find_target(int **dp,int r,int c,int t)
{
	for(int i=0;i<r;i++)
	{
		for(int j=0;j<c;j++)
		{
			if(dp[i][j] ==1)
			{
				return 1;
			}
		}
	}
	return 0;

}
int main()
{
	int **m,r,c,target=1;
	printf("enter rows,cols:");scanf("%d%d",&r,&c);
	m=(int**)malloc(r*sizeof(int*));
	for(int i=0;i<r;i++)
	{
		m[i]=(int *)malloc(c*sizeof(int));
	}
	for(int i=0;i<r;i++)
	{
		for(int j=0;j<c;j++)
		{
			scanf("%d",&m[i][j]);
		}
	}
	printf("mat print\n");
	for(int i=0;i<r;i++)
	{
		for(int j=0;j<c;j++)
		{
			printf("%d ",m[i][j]);
		}
		printf("\n");
	}
	if(find_target(m,r,c,target))
	{
		for(int i=0;i<r;i++)
		{
			for(int j=0;j<c;j++)
			{
				m[i][j]=1;
			}
		}
	}

	for(int i=0;i<r;i++)
	{
		for(int j=0;j<c;j++)
		{
			printf("%d ",m[i][j]);
		}
		printf("\n");
	}
	return 0;
}
/* Write a C program that takes a matrix as input and converts each row and column so that all values become 1 (set entire row & column to 1 if any element is 1).*/
