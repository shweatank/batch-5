#include<stdio.h>
void sorting(int a[],int size)
{
   int i,j,t;
  for(i=0;i<size;i++)
 {
    for(j=i+1;j<size;j++)
   {
     if(a[i]>a[j])
    {
      t=a[i];
      a[i]=a[j];
      a[j]=t;
    }
   }
 }
 for(i=0;i<size;i++)
 printf("%d ",a[i]);
}
int main()
{
  int a[]={11,44,3,55,6,22};
  int ele=sizeof(a)/sizeof(a[0]);
  int (*fun)(int* ,int)=sorting;
   fun(&a,ele);
}
         
