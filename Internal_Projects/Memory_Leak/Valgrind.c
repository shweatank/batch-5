#include<stdio.h>
#include<stdlib.h>


void memleak(int size)
{
	int *p=malloc(size*sizeof(int)),i;
	if(p==NULL)
		return;

	for(i=0;i<size;i++)
		p[i]=i;

	printf("we are returning from function without freeing allocated memory\n");
	return ;
}

int main()
{
	printf("we are trying to allocate memory\n");
	memleak(100);
}
