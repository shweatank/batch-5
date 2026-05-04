#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<fcntl.h>

void main()
{
int id=semget(2,2,IPC_CREAT|0664);
if(id<0)
{
perror("semget");
return;
}
printf("id=%d\n",id);

int fd=open("data",O_WRONLY|O_APPEND|O_CREAT,0664);
if(fd<0)
{
perror("open");
return;
}

printf("fd=%d\n",fd);

struct sembuf v;

v.sem_num=1;
v.sem_op=0;
v.sem_flg=0;

semctl(id,0,SETVAL,1);
semop(id,&v,2);

char ch;

for(ch='A';ch<='Z';ch++)
{
write(fd,&ch,1);
sleep(1);
}

semctl(id,0,SETVAL,0);
}
