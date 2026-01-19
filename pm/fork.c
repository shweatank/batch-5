#include"header.h"
int a;
void main()
{
if(fork()==0)
{
printf("pid=%d in child a=%d\n",getpid(),++a);
while(1);
}
else
{
//sleep(1);
printf("in parent %d\n",a);
}
}
