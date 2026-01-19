#include"header.h"
int c;
void ISR(int n)
{
c++;
printf("%d time executed\n",c);
if(c==3)
{
printf("no.of times executed ISR=%d\n",c);
exit(0);
}
}
void main()
{
signal(SIGINT,ISR);
while(1)
{
printf("pid=%d running\n",getpid());
sleep(1);
}
}
