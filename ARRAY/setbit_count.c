/*WAP in C to count all set and clear bit array .
i/p: int a[5]={10,15,64,100,511};
o/p: set count =19 , clear count=141*/
#include<stdio.h>
int main()
{

   int a[5]={10,15,64,100,511};
   int setcount=0;
   int clearcount=0,r,temp,pos;
   int ele=sizeof(a)/sizeof(a[0]);
   int i,j,k;
   for(i=0;i<ele;i++)
  {
    temp=a[i];
    for(pos=31;pos>=0;pos--)
   {
      r=(temp>>pos)&1;
      if(r)
     {
       setcount++;
     }
     else
    {
       clearcount++;
    }
   }
   
  }
  printf("setcount=%d clearcount=%d\n",setcount,clearcount);
}
