#include<stdio.h>
#include<stdlib.h>
void main()
{
	int r,c,**p,i,j;
	printf("enter the rows and ciolo..\n");
	scanf("%d %d",&r,&c);
	p=malloc(sizeof(int *)*r);
	for(i=0;i<r;i++)
		p[i]=malloc(sizeof(int)*c);
	printf("enter the elements..\n");
	for(i=0;i<r;i++)
	{
		for(j=0;j<c;j++)
		{
			scanf("%d",&p[i][j]);
		}
	}

	for(i=0;i<r;i++)
	{
		for(j=0;j<c;j++)
		{
			printf("%d ",p[i][j]);
			
		}
		printf("\n");
	}
for(i=0;i<r;i++)
    free(p[i]);

free(p);

}
