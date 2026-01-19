#include"header.h"
void ISR(int n)
{
printf("handled..\n");
}
void main()
{
printf("pid=%d\n",getpid());
signal(SIGINT,ISR);
kill(getpid(),SIGINT);
}
