#include"header.h"
int a[3],i;
void ISR(int n)
{
printf("child %d completed\n",++i);
}
void main()
{
srand(getpid());
if((a[0]=fork())==0)
{
int d=rand()%10;
printf("sleep %d\n",d);
sleep(d);
exit(0);
}
else
{
   if((a[1]=fork())==0)
{
int d=rand()%10;
printf("sleep %d\n",d);
sleep(d);
exit(1);
}
else
{
signal(SIGCHLD,ISR);
while(1);
} 
}
}
