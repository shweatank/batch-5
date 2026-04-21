#include<stdio.h>
#include<stdlib.h>
void main()
{
	int r,c,i,j;
	int **p;
	printf("enter the r and c values..\n");
	scanf("%d %d",&r,&c);
	p=malloc(r*sizeof(int *));
	for(i=0;i<r;i++)
		p[i]=malloc(c*sizeof(int));
	printf("enter the elements............\n");
	for(i=0;i<r;i++)
	{
		for(j=0;j<c;j++)
			scanf("%d",&p[i][j]);
	}

	for(i=0;i<r;i++)
	{
		for(j=0;j<c;j++)
			printf("%d ",p[i][j]);
		printf("\n");
	}

}
