#include<stdio.h>
#include<stdlib.h>
#include<time.h>
void Find(int *arr,int size)
{
	int cnt=0,f=0;
	for(int i=0;i<size;i++)
	{
		cnt=0;
		f=0;
		for(int k=0;k<i;k++)
		{
			if(arr[i]==arr[k])
			{
				f=1;
				break;
			}
		}
		if(f==1)
			continue;
		for(int j=i;j<size;j++)
		{
			if(arr[i]==arr[j])
				cnt++;
		}
		if(cnt>1)
			printf("%d ",arr[i]);
	}
}

void Print(int *arr,int size)
{
	for(int i=0;i<size;i++)
	{
		printf("%d ",arr[i]);
	}
	puts("");
}
int main()
{
	int arr[10];
	for(int row=0;row<10;row++)
	{
		scanf("%d ",arr+row);
	}
	Print(arr,10);
	Find(arr,10);
}
