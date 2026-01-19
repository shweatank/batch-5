#include"header.h"
void ISR(int n)
{
printf("before\n");
sleep(20);
printf("after\n");
}
void main()
{
printf("pid=%d\n",getpid());
struct sigaction*v=calloc(1,sizeof(struct sigaction));
v->sa_handler=ISR;
v->sa_flags=SA_NODEFER;
sigemptyset(&v->sa_mask);
sigaddset(&v->sa_mask,SIGQUIT);
sigaddset(&v->sa_mask,SIGFPE);
sigaction(SIGINT,v,0);
while(1);
}
