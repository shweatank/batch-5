#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
void main()
{
int r;
printf("pid=%d\n",getpid());
r=fork();
if(r==0)
{
printf("child\n");
printf("child pid=%d ppid=%d\n",getpid(),getppid());
//while(1);
//sleep(5);
exit(0);
}
else
{
printf("parent\n");
printf("pid=%d ppid=%d\n",getpid(),getppid());
//sleep(10);
int r=wait(0);
printf("r=%d\n",r);
}
}
