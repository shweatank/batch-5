#include"header.h"
#include<sys/sem.h>
pthread_t t1,t2;
int a[5],id;
struct sembuf v1;
int i;
void* producer(void*p)
{
printf("in producer\n");
while(1)
{
semop(id,&v1,1);
for(i=0;i<5;i++)
a[i]+=10;
semctl(id,0,SETVAL,1);
semctl(id,1,SETVAL,0);
}
pthread_exit(NULL);
}
void*consumer(void*p)
{
printf("in consumer\n");
while(1)
{
semop(id,&v1,1);
for(i=0;i<5;i++)
printf("%d\n",a[i]);
semctl(id,1,SETVAL,1);
semctl(id,0,SETVAL,0);
}
pthread_exit(NULL);
}
void mian()
{
id=semget(4,2,IPC_CREAT|666);
if(id<0)
{
perror("semget");
return ;
}
v1.sem_num=0;
v1.sem_num=1;
v1.sem_op=0;
v1.sem_flg=0;
semctl(id,0,SETVAL,0);
semctl(id,1,SETVAL,1);
pthread_create(&t1,NULL,producer,NULL);
pthread_create(&t2,NULL,consumer,NULL);
pthread_join(t1,NULL);
pthread_join(t2,NULL);
pthread_exit(NULL);
}
