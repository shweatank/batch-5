#include"header.h"
int p[2];
void ISR(int n)
{
printf("SIGPIPE signal is recieved\n");
}
void main()
{
pipe(p);
if(fork()==0)
{
int n,i;
close(p[0]);
struct sigaction v;
v.sa_handler=ISR;
v.sa_flags=0;
sigemptyset(&v.sa_mask);
sigaction(SIGPIPE,&v,0);
n=write(p[1],&i,4);
printf("%d\n",n);
while(1);
}
else
{
//int i;
//read(p[0],&i,4);
close(p[0]);
printf("hi\n");
while(1);
}
}
