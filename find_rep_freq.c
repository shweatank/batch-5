#include<stdio.h>
int main()
{
	int arr[10]={1,2,3,1,2,3,4,5,2,3},cnt=0;
	int frq[10]={0},n=10;
	for(int i=0;i<n;i++)
	{
		frq[arr[i]]++;
	}
	for(int i=0;i<n;i++)
	{
		if(frq[i]>1)
		{
			frq[i]=0;
			printf("%d ",arr[i]);
		}
	}
	
}
