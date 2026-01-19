#include"header.h"
int id;
void main()
{
id=semget(10,2,IPC_CREAT|0664);
if(id<0)
{
perror("semget");
return;
}
int fd=open("data",O_WRONLY|O_APPEND|O_CREAT,0664);
if(fd<0)
{
perror("open");
return;
}
struct sembuf v;
v.sem_num=0;
v.sem_op=0;
v.sem_flg=0;
for(char ch='a';ch<='z';ch++)
{
semop(id,&v,1);
semctl(id,1,SETVAL,1);
write(fd,&ch,1);
printf("%c is writed\n",ch);
semctl(id,0,SETVAL,1);
semctl(id,1,SETVAL,0);
}
printf("done\n");
}
