#include"header.h"
void main()
{
printf("hai pid=%d\n",getpid());
raise(SIGSTOP);
printf("hi\n");
while(1);
}
