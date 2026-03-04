#include<stdio.h>
int main()
{
	int arr[10];
	for(int row=0;row<10;row++)
	{
		scanf("%d ",arr+row);
	}
	int a[100]={};
	for(int row=0;row<10;row++)
	{
		if((++a[arr[row]])==2)
			printf("%d ",arr[row]);
	}
}
