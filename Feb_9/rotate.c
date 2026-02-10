#include<stdio.h>
int main(){
	int arr[]={1,2,3,4,5},d=2;
	int temp[d],i=0,n,j;
	n=sizeof(arr)/sizeof(arr[0]);
	for(i=0;i<d;i++)
		temp[i]=arr[i];
	memmove(arr+0,arr+2,sizeof(int)*(n-d));
	for(i=d+1,j=0;i<=n;i++,j++)
		arr[i]=temp[j];
	printf("rotated array is:");
	for(i=0;i<n;i++)
		printf("%d ",arr[i]);
}
