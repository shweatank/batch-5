#include"header.h"
void ISR(int n)
{
printf("after 5 seconds\n");
}
void main()
{
printf("pid=%d\n",getpid());
struct sigaction *v=calloc(1,sizeof(struct sigaction));
v->sa_handler=ISR;
v->sa_flags=0;
sigemptyset(&v->sa_mask);
sigaction(SIGALRM,v,0);
alarm(5);
//while(1);


}
