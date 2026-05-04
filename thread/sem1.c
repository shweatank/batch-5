#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<fcntl.h>
#include<sys/stat.h>
void main()
{
struct sembuf v;
int id=semget(3,5,IPC_CREAT|0664);
if(id<0)
{
perror("semget");
return;
}
perror("semget");
printf("id=%d\n",id);

int fd=open("data1",O_WRONLY|O_APPEND|O_CREAT,0664);
if(fd<0)
{
perror("open");
return;
}
printf("fd=%d\n",fd);
v.sem_num=0;
v.sem_op=0;
v.sem_flg=0;

semctl(id,0,SETVAL,0);
semop(id,&v,1);
//semctl(id,0,SETVAL,1);


char ch;

for(ch='A'; ch<='Z';ch++)
{
write(fd,&ch,1);
sleep(1);
}
semctl(id,0,SETVAL,1);
printf("done\n");

}


