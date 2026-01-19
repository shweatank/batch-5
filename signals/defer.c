#include"header.h"
void ISR(int n)
{
printf("before \n");
sleep(10);
printf("after\n");
}
void main()
{
struct sigaction *v=malloc(sizeof(struct sigaction));
v->sa_handler=ISR;
sigemptyset(&v->sa_mask);
v->sa_flags=SA_NODEFER;
sigaction(SIGINT,v,0);
while(1);
}
