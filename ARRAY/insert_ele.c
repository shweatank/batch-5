/*WAP to C to insert 10 on sorted place in sorted array
i/p: int a[7]={2,3,5,7,11,13 }, num= 10;
o/p: int a[7]={2,3,5,7, 10, 11,13};*/
#include<stdio.h>
int main()
{
   int a[7]={2,3,5,7,11,13 };
   int ele=sizeof(a)/sizeof(a[0]);
   int i,j,k,num=10;
 for(i=0;i<6;i++)
 printf("%d ",a[i]);
 printf("\n");
   for(i=0;i<ele;i++)
  {
    if(a[i]>num)
   {
     for(j=ele-1;j>=i;j--)
    {
       a[j+1]=a[j];
    }
    a[i]=num;
    
    break;
   }
 }
 for(i=0;i<ele;i++)
 printf("%d ",a[i]);
 printf("\n");
}    
