#include"header.h"
void* thread2(void *p){
	int semid=semget(1,2,IPC_CREAT|0664);
	int shmid=shmget(1,1024,IPC_CREAT|0664);
	struct sembuf v;
	v.sem_num=0;
	v.sem_op=0;
	v.sem_flg=0;
	semop(semid,&v,1);
	char *p1=shmat(shmid,0,0);	
	printf("data from thread 1 is %s\n",p1);
	shmdt(p1);
	pthread_exit(NULL);
}

void main(){
	int semid=semget(1,2,IPC_CREAT|0664);
	int shmid=shmget(1,1024,IPC_CREAT|0664);

	pthread_t t2;
	pthread_create(&t2,0,thread2,0);
	pthread_join(t2,NULL);
	struct sembuf v;
	semctl(semid,1,SETVAL,1);
	char *p=shmat(shmid,0,0);
	printf("Enter the data\n");
	scanf("%s",p);
	shmdt(p);
	semctl(semid,1,SETVAL,0);
}
