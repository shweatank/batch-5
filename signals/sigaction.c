#include"header.h"
void MY_ISR(int n)
{
printf("before sleep\n");
sleep(10);
printf("after sleep\n");
}
void main()
{
printf("hai pid=%d\n",getpid());
struct sigaction *v=malloc(sizeof(struct sigaction));
v->sa_handler=MY_ISR;
sigemptyset(&v->sa_mask);
v->sa_flags=0;
sigaction(SIGINT,v,0);
while(1);
}
