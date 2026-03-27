#include<stdio.h>
void swap(int *a,int *b)
{
   int t;
  t=*a;
  *a=*b;
  *b=t;
} 
int main()
{
  int a,b;
  printf("enter the a and b\n");
  scanf("%d %d",&a,&b);
  swap(&a,&b);
  printf("a=%d b=%d\n",a,b);
}      
     
