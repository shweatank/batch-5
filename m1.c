#include"header.h"
void* thread1(void *p){
int semid=semget(1,2,IPC_CREAT|0664);
int shmid=shmget(1,1024,IPC_CREAT|0664);
	semctl(semid,0,SETVAL,1);	//setting 0th sem as 1
	char *p1=shmat(shmid,0,0);	
	printf("Enter the data\n");
	scanf("%s",p1);
	shmdt(p1);
	semctl(semid,0,SETVAL,0);	//setting 0th sem as 0
pthread_exit(NULL);
}

void main(){
int semid=semget(1,2,IPC_CREAT|0664);
int shmid=shmget(1,1024,IPC_CREAT|0664);
	pthread_t t1;
	pthread_create(&t1,0,thread1,0);
pthread_join(t1,NULL);
	struct sembuf v;
	v.sem_num=1;
	v.sem_op=0;
	v.sem_flg=0;
	semctl(semid,1,SETVAL,1);	//setting 1st sem as 1
	semop(semid,&v,1);		// waiting to get 0 by another thread or process
	char *p=shmat(shmid,0,0);
	printf("Data from program2 thread2 is: %s\n",p);
	shmdt(p);
}
