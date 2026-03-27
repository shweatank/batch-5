#include<stdio.h>
int powerof2(int n)
{
  int r=n&(n-1);
  if(r==0)
  return 1;
  else
  return 0;
}  
int main()
{
  int num;
  printf("enter the num\n");
  scanf("%d",&num);
  int k=powerof2(num);
  if(k)
  printf("power of 2\n");
  else
  printf("not power of 2\n");
}
     
