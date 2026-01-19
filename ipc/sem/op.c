#include"header.h"
void main(int argc,char **argv)
{
int id,r;
id=semget(4,5,IPC_CREAT|0664);
if(id<0)
{
perror("semget");
return;
}
printf("id=%d\n",id);
struct sembuf v;
v.sem_num=2;
//v.sem_op=0;
//v.sem_op=10;
v.sem_op=-2;
v.sem_flg=SEM_UNDO;
printf("before\n");
semop(id,&v,1);
printf("after\n");
sleep(15);
printf("done\n");
}
