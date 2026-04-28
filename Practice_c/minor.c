#include<stdio.h>


int minor(int *p,int size)
{
	int i,fre[1000]={0},temp;
	for(i=0;i<size;i++)
	{
		fre[p[i]]++;
	}
	for(i=0;i<size;i++)
	{
		if(fre[p[i]]==1)
			return p[i];
	}
}


int main()
{
	int arr[10],i,ret;
	printf("enter the array elements:\n");
	for(int i=0;i<10;i++)
		scanf("%d",(arr+i));
	printf("before:\n");
	for(i=0;i<10;i++)
		printf("%d ",arr[i]);
	ret=minor(arr,10);
	printf("ret=%d\n",ret);
//	for(i=0;i<10;i++)
//		printf("%d ",arr[i]);
}
