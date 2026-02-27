#include<stdio.h>
#include<stdlib.h>
int main()
{
	int *p;
	
	*p=10;
	printf("%d\n",*p); //gives segmantation fault...i.e undefined behaviour
	/*
	int *d=NULL;
	d=(int *)malloc(2*sizeof(int));d[0]=10;d[1]=20;
	//free(d);
	//printf("%d",*d);*/
	return 0;
}
