#include"header.h"
int shid,semid;
char *p,*q;
struct sembuf v;
void main()
{
	shid=shmget(10,100,IPC_CREAT|0644);
	if(shid<0)
	{
		perror("shmget");
		return;
	}
	semid=semget(15,3,IPC_CREAT|0644);
	if(semid<0)
	{
		perror("semget");
		return;
	}
	semctl(semid,0,SETVAL,1);
	if(fork()==0)
	{
		v.sem_num=0;
		v.sem_op=0;
		v.sem_flg=0;
		semop(semid,&v,1);
		q=shmat(shid,0,0);
		printf("child1 %s\n",q);
	}
	else
	{
		p=shmat(shid,0,0);
		printf("enter string:");
		scanf("%s",p);
		semctl(semid,0,SETVAL,0);
		wait(0);
	}
}
