#include"header.h"
void ISR(int n)
{
printf("SIGINT is recieved\n");
}
void main()
{
signal(SIGINT,ISR);
if(fork()==0)
{
printf("chld=%d par=%d\n",getpid(),getppid());
pause();
printf("child recieved\n");
}
else
{
printf("in parent=%d\n",getpid());
pause();
printf("parent recieved\n");
}
}
