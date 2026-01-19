#include"header.h"
int pid;
void ISR(int n)
{
printf("SIGUSR1 is handled by child \n");
}
void main()
{
if((pid=fork())==0)
{
printf("child is waiting for SIGUSR1 sent by parent\n");
signal(SIGUSR1,ISR);
pause();
}
else
{
sleep(3);
kill(pid,SIGUSR1);
}


}
