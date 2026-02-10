#include<stdio.h>
#include<stdlib.h>
void main(){

	int rows,col,**a,**b;
	printf("Enter no.of rows and coloumns\n");
	scanf("%d%d",&rows,&col);
	a=malloc(sizeof(int*)*rows);
	b=malloc(sizeof(int*)*rows);
	for(int i=0;i<rows;i++){
		a[i]=malloc(sizeof(int)*col);
		b[i]=malloc(sizeof(int)*col);
	}

	printf("Enter elements of matrix a and matrix b\n");
	for(int i=0;i<rows;i++)
	{
		for(int j=0;j<col;j++)
			scanf("%d",&a[i][j]);
	}

	for(int i=0;i<rows;i++)
	{
		for(int j=0;j<col;j++)
			scanf("%d",&b[i][j]);
	}

	int c[rows][col];
	for(int i=0;i<rows;i++){
		for(int j=0;j<col;j++)
			c[i][j]=0;
	}
	for(int i=0;i<rows;i++){
		for(int j=0;j<col;j++){
			for(int k=0;k<col;k++){
				c[i][j]=(a[i][k]*b[k][j])+c[i][j];
			}

		}
	}

	printf("Result is : \n");
	for(int i=0;i<rows;i++){
		for(int j=0;j<col;j++)
			printf("%d ",c[i][j]);
		printf("\n");
	}
}


