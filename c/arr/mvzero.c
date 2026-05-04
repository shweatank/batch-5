#include<stdio.h>
void main()
{
int arr[5],ele,i,j,temp;
ele=sizeof(arr)/sizeof(arr[0]);
printf("enter arr eles\n");
for(i=0;i<ele;i++)
scanf("%d",&arr[i]);
printf("before\n");
for(i=0;i<ele;i++)
printf("%d ",arr[i]);
printf("\n");
for(i=0,j=ele-1;i<j;i++,j--)
{
if((arr[i]==0 && arr[j]==1)||(arr[i]==1||arr[j]==0))
{
temp=arr[i];
arr[i]=arr[j];
arr[j]=temp;
}
}
printf("after\n");
for(i=0;i<ele;i++)
printf("%d ",arr[i]);
}
