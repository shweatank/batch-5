#include<stdio.h>
int main()
{
	int arr[10]={1,2,1,4,3,4,2,3,5,1};
	int b[100]={};
	for(int i=0;i<10;i++)
	{
		if((++b[arr[i]])==2)
			printf("%d ",arr[i]);

	}

}
