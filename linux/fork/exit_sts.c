#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
void main()
{
int r;
r=fork();
if(r==0)
{
printf("child\n");
printf("pid=%d,ppid=%d\n",getpid(),getppid());
exit(1);
}
else
{
printf("parent\n");
int r,s;
r=wait(&s);
printf("status=%d\n",s);
}
}
