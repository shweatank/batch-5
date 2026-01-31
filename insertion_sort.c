#include<stdio.h>
int main()
{
  int a[]={5,4,3,2,1};
  int ele=sizeof(a)/sizeof(a[0]);
  int i,j,k,temp;
  for(i=0;i<ele;i++)
  printf("%d ",a[i]);
  printf("\n");
  i=1;
 while(i<ele)
 {
   j=i-1;
   temp=a[i];
   while(j>=0&&a[j]>temp)
  {
     a[j+1]=a[j];
     j--; 
  }
  a[j+1]=temp;
  i++;
 }
 for(i=0;i<ele;i++)
 printf("%d ",a[i]);
}                            
