#include"header.h"
void main()
{
printf("hai pid=%d\n",getpid());
kill(getpid(),SIGSTOP);
signal(SIGSTOP,SIG_DFL);
while(1);
}
