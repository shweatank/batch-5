#include<stdio.h>
int main()
{
	int k=0,arr[10],i,j,flag;
	printf("enter the array elements:");
	for(i=0;i<n;i++)
		scanf("%d",arr+i);
	for(i=0;i<n;i++)
	{
		flag=0;
		for(j=0;j<k;j++)
		{
			if(arr[i]==arr[j])
			{
				flag=1;
				break;
			}
		}
		if(!flag)
			arr[k++]=arr[i];
	}
	printf("after removing:\n");
	for(i=0;i<k;i++)
		printf("%d ",arr[i]);

}
