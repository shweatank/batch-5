#include"header.h"
void main()
{
int id=semget(1,2,IPC_CREAT|0664);
if(id<0)
{
perror("semget");
return;
}

int fd=open("d",O_WRONLY|O_APPEND|O_CREAT,0664);
if(fd<0)
{
perror("open");
return;
}

struct sembuf v;
 sem_num=0;
sem_op=0;
sem_flg=0;

for(int i=0;i<10;i++)
{
semop(id,&v,1);
semctl(id,1,SETVAL,1);
write(fd,&i,4);
semctl(id,1,SETVAL,0);
semctl(id,0,SETVAL,1);
}
}
