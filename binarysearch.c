#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int main()
{
  int i,j,k;
  int a[]={1,2,3,4,5,6,7,8};
  int ele=sizeof(a)/sizeof(a[0]);
  j=ele-1;
 int target;
  printf("enter the target\n");
  scanf("%d",&target);
 int mid;
 i=0;
 while(i<=j)
{
 
  mid=(i+j)/2;
 if(a[mid]==target)
 {
   printf("%d %d\n",a[mid],mid);
   return 0;
 }
 if(a[mid]<=target)
{
   i=mid+1;
}
else
{
  j=mid-1;
}
}
}                
