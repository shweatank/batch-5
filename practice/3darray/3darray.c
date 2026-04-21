#include<stdio.h>
#include<stdlib.h>

void main()
{
	int r,c,l;
	int i,j,k;
	int ***p;
	printf("enter the r ,c and l values..\n");
	scanf("%d %d %d",&r,&c,&l);

	p=malloc(r*sizeof(int **));
			for(i=0;i<r;i++)
			{
			p[i]=malloc(c*sizeof(int *));
			for(j=0;j<c;j++)
				p[i][j]=malloc(l*sizeof(int));
			}
			printf("nter the elements..\n");
			for(i=0;i<r;i++)
			{
				for(j=0;j<c;j++)
				{
					for(k=0;k<l;k++)
						scanf("%d",&p[i][j][k]);
				}
			}
			for(i=0;i<r;i++)
                        {
                                for(j=0;j<c;j++)
                                {
                                        for(k=0;k<l;k++)
                                                printf("%d ",p[i][j][k]);
					printf("\n");
                                }
				printf("\n");
                        }

}
