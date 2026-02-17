/*26. WAP in C to delete 2nd,3rd index from array .
i/p: int a[5]={99,88,77,66,55}, pos1=2, pos2=3;
o/p: int a[5]={99,88,55};*/
#include<stdio.h>
int main()
{
   int a[]={99,88,77,66,55};
   int ele=5;
   int i,j;
   for(i=0;i<ele;i++)
   printf("%d ",a[i]);
   printf("\n");
   int pos,k=0;
  l:
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
  if(k<1)
  {
    k++;
    goto l;
  }
   for(i=0;i<ele;i++)
   printf("%d ",a[i]);
   printf("\n");
}

 
