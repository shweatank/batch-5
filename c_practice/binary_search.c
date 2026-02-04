#include<stdio.h>

void search(int *arr,int size)
{
	int low,high,mid,ele;
	printf("enter element : ");
	scanf("%d",&ele);
	high=size-1;
	low=0;
	while(low<=high)
	{
		mid=(low+high)/2;
		if(arr[mid]==ele)
		{
			printf("element  : %d  index %d\n",arr[mid],mid);
			break;
		}
		else if(ele<arr[mid])
			high=mid-1;
		else
			low=mid+1;
	}
}

void init_array(int *arr,int size)
{
	for(int i=0;i<size;i++)
	{
		printf("enter element : ");
		scanf("%d",&arr[i]);
	}
}

void print_array(int *arr,int size)
{
	for(int i=0;i<size;i++)
	{
		printf("%d\n",arr[i]);
	}
	printf("\n");
}

int main()
{
	int arr[10];
	init_array(arr,10);
	print_array(arr,10);
	search(arr,10);
}
