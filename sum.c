#include<stdio.h>
void main()
{
	int arr1[5],arr2[5],sumarr[5],i;
	for(i=0;i<5;i++)
	{
		scanf("%d",&arr1[i]);
	}
	for(i=0;i<5;i++)
	{
		scanf("%d",&arr2[i]);
	}
	for(i=0;i<5;i++)
	{
		sumarr[i]=arr1[i]+arr2[i];
		printf("sum array elements at index %d is :%d\n",i,sumarr[i]);
	}
}

