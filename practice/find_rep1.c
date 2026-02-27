#include<stdio.h>
int main()
{
	int arr[7]={2,4,1,1,1,2,4},cnt=0;
	for(int i=0;i<7;i++)
	{
		cnt=0;
		for(int j=0;j<7;j++)
		{
			if((arr[i]==arr[j]) &&(i>j))
				break;
			if(arr[i]==arr[j])
				cnt++;
		}
		if(cnt>1)
			printf("%d ",arr[i]);
	}
	return 0;
}
