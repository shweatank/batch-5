#include<stdio.h>
#include<stdlib.h>
void DMA(int***);
void main(){
	int **p;
	DMA(&p);
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<4;j++)
			scanf("%d",&p[i][j]);
	}

	for(int i=0;i<3;i++)
	{
		for(int j=0;j<4;j++)
			printf("%d ",p[i][j]);
		printf("\n");
	}
	int (*a)[4];
	DMA1(&a);
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<4;j++)
			scanf("%d",&a[i][j]);
	}

	for(int i=0;i<3;i++)
	{
		for(int j=0;j<4;j++)
			printf("%d ",a[i][j]);
		printf("\n");
	}
	int *b[3];
	DMA2(&b);
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<4;j++)
			scanf("%d",&b[i][j]);
	}

	for(int i=0;i<3;i++)
	{
		for(int j=0;j<4;j++)
			printf("%d ",b[i][j]);
		printf("\n");
	}
}

void DMA(int***p){

	*p=malloc(sizeof(int*)*3);
	for(int i=0;i<4;i++)
		(*p)[i]=malloc(sizeof(int)*4);
}
void DMA1(int ***p){
	*p=malloc(sizeof(int*)*3);
}

void DMA2(int **p){

	for(int i=0;i<3;i++)
p[i]=malloc(sizeof(int)*4);
}
