#include<stdio.h>

void find_zero(int *p,int size)
{
	int cnt=0,i;
	for(i=0;i<size;i++)
	{
		if(p[i]!=0)
		{
			p[cnt++]=p[i];
		}
	}
	for(i=cnt;i<size;i++)
		p[i]=0;
}


int main()
{
	int arr[10],i;
	printf("enter the array elements:\n");
	for(int i=0;i<10;i++)
		scanf("%d",(arr+i));
	printf("before:\n");
	for(i=0;i<10;i++)
		printf("%d ",arr[i]);
	find_zero(arr,10);
	printf("modified:\n");
	for(i=0;i<10;i++)
		printf("%d ",arr[i]);
}
