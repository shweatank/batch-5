#include"header.h"
struct msg
{
long mtype;
char data[20];
}v;
void main(int argc,char**argv)
{
if(argc!=3)
{
printf("usage: ./a.out mtype data\n");
return;
}
int id;
id=msgget(10,IPC_CREAT|0664);
if(id<0)
{
perror("msgget");
return;
}
v.mtype=atoi(argv[1]);
strcpy(v.data,argv[2]);
msgsnd(id,&v,strlen(v.data)+1,0);
perror("msgsend");
}

