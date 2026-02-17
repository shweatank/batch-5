/*2. WAP in C to shift all -ve elements to right side in
same array.
i/p: int a[7]={-1,22,3,-4,-8,33,77};
o/p: int a[7]={22,3,33,77,-1,-4,-8,};*/
#include<stdio.h>
int main()
{
   int a[7]={-1,22,3,-4,-8,33,77};
   int ele=sizeof(a)/sizeof(a[0]);
   int i,j;
   for(i=0;i<ele;i++)
   printf("%d ",a[i]);
   printf("\n");
   int t,k=0;
   for(i=0;i<ele-k;i++)
  {
    if(a[i]<0)
    {
     k++;
     t=a[i];
     for(j=i;j<ele;j++)
     {
       a[j]=a[j+1];
     }
     a[ele-1]=t;
     i--;
    }
  }   
   for(i=0;i<ele;i++)
   printf("%d ",a[i]);
   printf("\n");
}
