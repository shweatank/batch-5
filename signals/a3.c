#include"header.h"
struct sigaction *v;
void ISR(int n)
{
printf("you have to enter in 10 seconds\n");

}
void main()
{
printf("pid=%d\n",getpid());
v=calloc(1,sizeof(struct sigaction));
v->sa_handler=ISR;
v->sa_flags=0;
sigemptyset(&v->sa_mask);
sigaction(SIGALRM,v,0);
alarm(10);
int n;
printf("enter the num:");
scanf("%d",&n);
while(1);


}
