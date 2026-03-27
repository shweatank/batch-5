/*WAP in C to count all duplicate elements from array.
i/p: a[10]={3,3,2,4,4,2,5,3,4,9}
o/p: 3 ->3 times ,2-> 2 times , 4-> 3 time*/
#include<stdio.h>
int main()
{
   int  a[10]={3,3,2,4,4,2,5,3,4,9};
   int ele=sizeof(a)/sizeof(a[0]);
   int i,j,k,flag,c;
   for(i=0;i<ele;i++)
  {
    flag=0;
    c=0;
    for(j=i-1;j>=0;j--)
   {
     if(a[i]==a[j])
    {
     flag=1;
     break;
    }
   }
   if(flag==0)
  {
    for(k=i;k<ele;k++)
   {
     if(a[i]==a[k])
    {
      c++;
    }     
   }
   if(c>1)
  {
   printf("%d c=%d->times ",a[i],c);
  }
  }
} 
}  
   

