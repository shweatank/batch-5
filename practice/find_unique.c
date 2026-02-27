#include<stdio.h>
int main()
{
	int arr[6]={1,2,1,2,3,4},cnt=0;
for(int i=0;i<6;i++)
{
	cnt=0;
	for(int j=0;j<6;j++)
	{
		if((arr[i]==arr[j])&&(i>j))
			break;
		if(arr[i]==arr[j])
			cnt++;
	}
	if(cnt >=1)
		printf("%d ",arr[i]);
}	
}
