#include"header.h"
struct sembuf p[2];
int id;
void*thread2_write(void*v)
{
p[1].sem_num=1;
p[1].sem_op=0;
p[1].sem_flg=0;
for(int i='a';i<='z';i++)
{
semop(id,&p[1],1);
semctl(id,0,SETVAL,1);
printf("%c",i);
fflush(stdout);
semctl(id,1,SETVAL,1);
semctl(id,0,SETVAL,0);
}
pthread_exit(0);
}
void*thread1_write(void*v)
{
p[0].sem_num=0;
p[0].sem_op=0;
p[0].sem_flg=0;
for(int i='A';i<='Z';i++)
{
semop(id,&p[0],1);
semctl(id,1,SETVAL,1);
printf("%c",i);
fflush(stdout);
semctl(id,0,SETVAL,1);
semctl(id,1,SETVAL,0);
}
pthread_exit(0);
}
void main()
{
id=semget(2,2,IPC_CREAT|0664);
if(id<0)
{
perror("semget");
exit(0);
}
semctl(id,0,SETVAL,0);
semctl(id,1,SETVAL,1);
pthread_t t1,t2;
pthread_create(&t1,NULL,thread1_write,NULL);
pthread_create(&t2,NULL,thread2_write,NULL);
pthread_join(t1,NULL);
pthread_join(t2,NULL);
pthread_exit(0);
}
