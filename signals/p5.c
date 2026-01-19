#include"header.h"
int pid;
void ISR(int n)
{
printf("SIGINT is recieved by child\n");
exit(0);
}
void main()
{
if((pid=fork())==0)
{
printf("chld=%d par=%d\n",getpid(),getppid());
signal(SIGINT,ISR);
while(1)
{
printf("child is running..\n");
sleep(1);
}

}
else
{
printf("in par pid=%d\n",getpid());
sleep(5);
kill(pid,SIGINT);
}

}
