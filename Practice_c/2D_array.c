#include<stdio.h>
#include<stdlib.h>

int main()
{
	int **p;
	p=malloc(sizeof(p)*4);
	for(int i=0;i<4;i++)
		p[i]=malloc(sizeof(int)*5);
	for(int i=0;i<4;i++)
		for(int j=0;j<5;j++)
			p[i][j]=i+j;
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<5;j++)
			printf("%d ",p[i][j]);
		printf("\n");
	}
}
