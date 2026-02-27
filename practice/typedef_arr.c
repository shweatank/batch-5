#include<stdio.h>
typedef int arr[10];
int main()
{
	arr array;
	for(int i=0;i<10;i++)
	{
		array[i]=i+1;
	}
	for(int i=0;i<10;i++)
		printf("%d ",array[i]);
	printf("\n");
	return 0;
}
