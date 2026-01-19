#include"header.h"
void main()
{
if(fork()==0)
{
printf(" child pid=%d  %d\n",getpid(),getppid());
exit(0);
return ;
}
else
{
printf("in par %d\n",getpid());
wait(0);
while(1);
}
}
