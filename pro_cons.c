#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<sys/sem.h>
pthread_t t1,t2,t3;
struct sembuf v1={0,0,0};
struct sembuf v2={1,0,0};
struct sembuf v3={2,0,0};
int semid;
void * thread1(void*p)
{
while(1)
{
semop(semid,&v1,1);
printf("thread1\n");
sleep(1);
semctl(semid,0,SETVAL,1);
semctl(semid,1,SETVAL,0);
semctl(semid,2,SETVAL,1);
}
}
void * thread2(void*p)
{
while(1)
{
semop(semid,&v2,1);
printf("thread2\n");
sleep(1);
semctl(semid,1,SETVAL,1);
semctl(semid,2,SETVAL,0);
semctl(semid,0,SETVAL,1);
}
}
void * thread3(void*p)
{
while(1)
{
semop(semid,&v3,1);
printf("thread3\n");
sleep(1);
semctl(semid,2,SETVAL,1);
semctl(semid,1,SETVAL,1);
semctl(semid,0,SETVAL,0);
}
}
void main()
{
semid=semget(4,3,IPC_CREAT|0666);
if(semid<0)
{
perror("semget");
exit(0);
}
semctl(semid,0,SETVAL,0);
semctl(semid,1,SETVAL,1);
semctl(semid,2,SETVAL,1);
pthread_create(&t1,0,thread1,NULL);
pthread_create(&t2,0,thread2,NULL);
pthread_create(&t3,0,thread3,NULL);
pthread_join(t1,NULL);
pthread_join(t2,NULL);
pthread_join(t3,NULL);
}
