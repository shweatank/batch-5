#include"header.h"
void ISR(int n)
{
printf("signal recieved\n");
kill(getpid(),SIGTERM);
}
void main()
{
printf("pid=%d\n",getpid());
signal(SIGINT,ISR);
while(1)
{
printf("running...\n");
sleep(2);
}
}
