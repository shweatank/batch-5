/*WAP in C to delete 0th index element from array
i/p: int a[5]={99,88,77,66,55}, in=0;
o/p: int a[5]={88,77,66,55};*/
#include<stdio.h>
int main()
{
   int a[5]={99,88,77,66,55};
   int ele=sizeof(a)/sizeof(a[0]);
   int i,j,k;
   for(i=0;i<ele;i++)
   printf("%d ",a[i]);
   printf("\n");
   int pos;
   printf("enter the pos to delte\n");
   scanf("%d",&pos);
   for(i=0;i<ele;i++)
  {
    if(i==pos)
   {
     for(j=i;j<ele;j++)
    {
       a[j]=a[j+1];
    }
    ele--;
    break;
   }
  }
   for(i=0;i<ele;i++)
   printf("%d ",a[i]);
   printf("\n");
}

 
