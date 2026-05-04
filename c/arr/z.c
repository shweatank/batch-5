#include<stdio.h>
void main()
{
int arr[5],i,j,ele;
ele=sizeof(arr)/sizeof(arr[0]);
for(i=0;i<ele;i++)
scanf("%d",&arr[i]);
//i=0;
j=ele-1;
int temp;
for(i=0;i<ele;i++)
{
if(arr[i]==1)
{
if(arr[j]==0)
{
temp=arr[i];
arr[i]=arr[j];
arr[j]=temp;
}
else
{
j--;
i++;
}
}
}
for(i=0;i<ele;i++)
printf("%d ",arr[i]);
}
