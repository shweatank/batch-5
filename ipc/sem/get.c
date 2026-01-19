#include"header.h"
void main(int argc,char **argv)
{
if(argc!=2)
{
printf("./a.out semnum\n");
return;
}
int id,r;
//id=semget(4,5,IPC_CREAT|0664);
id=semget(10,2,IPC_CREAT|0664);
if(id<0)
{
perror("semget");
return;
}
r=semctl(id,atoi(argv[1]),GETVAL);
if(r<0)
{
perror("semctl");
return;
}
printf("id=%d r=%d\n",id,r);
}
