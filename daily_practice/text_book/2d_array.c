#include<stdio.h>
#include<stdlib.h>

#define ROW 6
#define COL 12
int main()
{
	char **d=NULL;
	d=malloc(ROW*sizeof(char *));
	for(int i=0;i<ROW;i++)
	{
		d[i]=malloc(COL*sizeof(char));
	}
	for(int i=0;i<ROW;i++)
	{
			scanf("%s",d[i]);
	}
	for(int i=0;i<ROW;i++)
	{
			printf("%s\n",d[i]);
	}
}
