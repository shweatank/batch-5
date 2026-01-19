#include"header.h"
struct msg
{
long mtype;
char data[20];
}v;
void main(int argc,char**argv)
{
if(argc!=2)
{
printf("usage: ./a.out mtype\n");
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
msgrcv(id,&v,sizeof(v.data),v.mtype,0);
printf("%s\n",v.data);
}

