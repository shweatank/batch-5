#include<stdio.h>
int main()
{
   char *p=10;
   printf("%d\n",(char*)(p+1)-(char*)p);

}  
