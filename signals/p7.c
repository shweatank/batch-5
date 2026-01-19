#include"header.h"
int c_pid;
void main()
{
if((c_pid=fork())==0)
{
while(1)
{
printf("child is running pid=%d\n",getpid());
sleep(1);
}
}
else
{
sleep(10);
kill(c_pid,SIGKILL);
printf("parent killed child\n");
}
}
