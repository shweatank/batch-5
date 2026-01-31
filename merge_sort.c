#include<stdio.h>
void mergesort(int a[],int b[],int lb,int mid,int up) 
{
	int i=lb;
	int j=mid+1;
	int k=lb;
	while(i<=mid&&j<=up)
	{
		if(a[i]<=a[j])
		{
			b[k]=a[i];
			i++;
			k++;
		}
		else
		{
			b[k]=a[j];
			j++;
			k++;
		}
	}
	if(i>mid)
	{
		while(j<=up)
		{
			b[k]=a[j];
			k++;
			j++;
		}
	}
	else
	{
		while(i<=mid)
		{
			b[k]=a[i];
			i++;
			k++;
		}
	}
 for(k=lb;k<=up;k++) 
 a[k]=b[k]; 
}      
void merge_sort(int a[],int b[],int lb,int up)
{
  int mid;
  if(lb<up)
 {
   mid=(lb+up)/2;
  merge_sort(a,b,lb,mid);
  merge_sort(a,b,mid+1,up);
  mergesort(a,b,lb,mid,up);
 }
}
int main()
{
   int a[]={0,33,1,55,3,44,88,2,1,9,2,2};
   int ele=sizeof(a)/sizeof(a[0]);
   int b[ele]; 
  int i;
  for(i=0;i<ele;i++)
  printf("%d ",a[i]);
  printf("\n");
  merge_sort(a,b,0,ele-1);
  for(i=0;i<ele;i++)
  printf("%d ",a[i]);
}    
