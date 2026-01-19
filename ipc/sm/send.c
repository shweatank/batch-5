#include"header.h"
void main()
{
int id;
id=shmget(10,50,IPC_CREAT|0664);
if(id<0)
{
perror("shmget");
exit(0);
}
int *p;
p=shmat(id,0,0);
printf("%p\n",p);
if(p<0)
{
perror("shmat");
exit(0);
}
int i;
for(i=0;i<3;i++)
scanf("%d",&p[i]);
if(shmdt(p)<0)
{
perror("shmdt");
return;
}
shmdt(p);
}
