#include"header.h"
void main()
{
printf("pid=%d\n",getpid());
signal(SIGINT,SIG_IGN);
while(1)
{
printf("running\n");
sleep(1);
}
}
