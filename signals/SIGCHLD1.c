#include"header.h"
int pid;
struct sigaction *v;
void SIGCHLD_ISR(int n)
{
printf("recieved\n");
}
void main()
{
if((pid=fork())==0)
{
printf("chld=%d par=%d\n",getpid(),getppid());
sleep(20);
exit(0);
}
else
{
v=calloc(1,sizeof(struct sigaction));
v->sa_handler=SIGCHLD_ISR;
sigemptyset(&v->sa_mask);
v->sa_flags=SA_NOCLDSTOP|SA_NOCLDWAIT|SA_NODEFER;
sigaction(SIGCHLD,v,0);
printf("parent is waiting for SIGCHLD signal %d\n",getpid());
while(1);
}
}
