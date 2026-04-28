#include<stdio.h>
#include<stdlib.h>
int *move(int *p, int size)
{
	int i=0,j=size-1;
	while(i<j)
	{
		if(p[i]==0)
		{
			while(i<j)
			{
				if(p[j]==1)
				{
					p[i]=1;
					p[j]=0;
					j--;
					break;
				}
				j--;
				
			}
		}
		i++;
	}
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
	move(arr,10);
	printf("modified:\n");
	for(i=0;i<10;i++)
		printf("%d ",arr[i]);
}
