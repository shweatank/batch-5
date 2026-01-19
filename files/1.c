#include"header.h"
void main()
{
if(vfork()==0)
{
printf("chd=%d par =%d\n",getpid(),getppid());
exit(0);
}
else
{
wait(0);
printf("par %d\n",getpid());
while(1);
}
}
