#include"header.h"
int a=10;
void main()
{
printf("hi\n");
if(vfork()==0)
{
a=20;
printf("in child a=%d\n",a);
while(1);
}
else
printf("in parent a=%d\n",a);
}
