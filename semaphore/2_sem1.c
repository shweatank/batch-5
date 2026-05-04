#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<unistd.h>
#include<fcntl.h>

void main()
{
int id=semget(7,2,IPC_CREAT|0664);
if(id<0)
{
perror("semget");
return;
}

int fd=open("2sem",O_WRONLY|O_CREAT|O_APPEND,0664);
{
if(fd<0)
{
perror("open");
return;
}
}

struct sembuf v;

v.sem_num=1;
v.sem_op=0;
v.sem_flg=0;

char ch;
//semctl(id,0,SETVAL,1);
for(ch='A'; ch<='Z'; ch++)
{
semop(id,&v,1);
semctl(id,0,SETVAL,1);
write(fd,&ch,1);
semctl(id,0,SETVAL,0);
semctl(id,1,SETVAL,1);
//sleep(1);
}
printf("DONE\n");
}
