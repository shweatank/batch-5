#include<stdio.h>
int main()
{
  int a[10]={11,22,33,44,55};
  int ele=5;
  int i,k;
  for(i=0;i<ele;i++)
  printf("%d ",a[i]);
  printf("\n");
  int num,pos,j;
 for(j=0;j<2;j++)
{
  printf("enter the num\n");
  scanf("%d",&num);
  printf("enter the pos\n");
  scanf("%d",&pos);
  for(i=ele-1;i>=pos;i--)
 {
    a[i+1]=a[i];
 }
 ele++;
 a[pos]=num;
 for(i=0;i<ele;i++)
 printf("%d ",a[i]);
 printf("\n");
} 
/*
 for(i=0;i<ele;i++)
 printf("%d ",a[i]);
 printf("\n");*/
}    
