#include"header.h"
void ISR(int n)
{
printf("before..\n");
sleep(10);
printf("after..\n");
}
void main()
{
printf("hai pid=%d\n",getpid());
signal(SIGINT,ISR);
int p=pause();
printf("hello %d\n",p);
while(1);
}
