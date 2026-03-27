#include<stdio.h>
int powerof4(int n)
{
   if(n<0)
   return 0;
   while(n%4==0)
  {
    n=n/4;
  }
  return (n==1);
}
int main()
{
  int num;
  printf("enter the num\n");
  scanf("%d",&num);
  int k=powerof4(num);
  if(k)
  printf("power of 4\n");
  else
  printf("not power of 4\n");
}
      
