/*.WAP in C to delete 1st digit of all array elements.
i/p: int a[6]={12,234,321,45,654,1234};
o/p: int a[6]={2 , 34, 21, 5, 54, 234};*/
#include<stdio.h>
int main()
{
  int a[6]={12,234,321,45,654,1234};
  int ele=sizeof(a)/sizeof(a[0]);
  int i,j,k;
  int temp,num,rev,rev1,d;
  for(i=0;i<ele;i++)
 {
    for(temp=a[i],rev=0;temp>0;temp/=10)
   {
     
     rev=rev*10+temp%10;
   }
   rev=rev/10;
   for(temp=rev,rev1=0;temp>0;temp/=10)
  {
     rev1=rev1*10 + temp%10;
  }
  printf("%d ",rev1);     
 } 
}
       
