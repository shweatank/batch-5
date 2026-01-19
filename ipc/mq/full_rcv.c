#include"header.h"
struct msg
{
long mtype;
char data[20];
}v;
void main()
{
int id;
id=msgget(1,IPC_CREAT|0664);
if(id<0)
{
perror("msgget");
return;
}
if(fork()==0)
{
while(1)
{
printf("in child enter string for mtype 3\n");
scanf("%s",v.data);
v.mtype=3;
msgsnd(id,&v,strlen(v.data)+1,0);
}
}
else
{
while(1)
{
msgrcv(id,&v,sizeof(v.data),2,0);
printf(" recieving data from mtype 2=%s\n",v.data);
}
}
}

