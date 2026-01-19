#include"header.h"
void main()
{
struct rlimit *v=calloc(1,sizeof(struct rlimit));
if(getrlimit(RLIMIT_STACK,v)<0)
{
perror("getrlimit");
return;
}
perror("getrlimit(stk size)");
printf("stack SL:%u HL=%u\n",v->rlim_cur,v->rlim_max);
v->rlim_cur=10;
if(setrlimit(RLIMIT_CPU,v)<0)
{
perror("setrlimit");
return;
}
perror("setrlimit");
printf("pid=%d\n",getpid());
while(1);
}
