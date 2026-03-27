/*WAP in C to find sum of 1st digit of all array
i/p: int a[5]={123,23,89,3425,721};
o/p: 1+2+8+3+7 ----> 21*/
#include<stdio.h>
int main()
{
  int a[5]={123,23,89,3425,721};
  int ele=sizeof(a)/sizeof(a[0]);
  int i,j,k,sum=0,temp;
  for(i=0;i<ele;i++)
 {
   for(temp=a[i];temp>10;temp/=10);
   temp=temp%10;
   sum=sum+temp;
 }
 printf("sum=%d\n",sum); 
} 
  
