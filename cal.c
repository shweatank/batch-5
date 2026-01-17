#include"header.h"
int semid,res[4],key=1,a=10,b=5;
void*add(void *p){
	struct sembuf v={0,0,0};
	res[0]=a+b;
	semctl(semid,1,SETVAL,0);
	pthread_exit(NULL);

}
void *sub(void*p){
	struct sembuf v={1,0,0};
	semop(semid,&v,1);
	res[1]=a-b;
	semctl(semid,2,SETVAL,0);
	pthread_exit(NULL);

}
void *mul(void *p){
	struct sembuf v={2,0,0};
	semop(semid,&v,1);
	res[2]=a*b;
	semctl(semid,3,SETVAL,0);
	pthread_exit(NULL);
}
void *divi(void *p){
	struct sembuf v={3,0,0};
	semop(semid,&v,1);
	res[3]=a/b;
	semctl(semid,4,SETVAL,0);
	pthread_exit(NULL);
}

void main(){

	pthread_t t1,t2,t3,t4;
	semid=semget(key,5,IPC_CREAT|0664);
	struct sembuf v={4,0,0};
	semctl(semid,0,SETVAL,0);
	semctl(semid,1,SETVAL,1);
	semctl(semid,2,SETVAL,1);
	semctl(semid,3,SETVAL,1);
	semctl(semid,4,SETVAL,1);
	pthread_create(&t1,0,add,0);
	pthread_create(&t2,0,sub,0);
	pthread_create(&t3,0,mul,0);
	pthread_create(&t4,0,divi,0);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	pthread_join(t3,NULL);
	pthread_join(t4,NULL);
	semop(semid,&v,1);

	printf("add=%d\n",res[0]);
	printf("sub=%d\n",res[1]);
	printf("mul=%d\n",res[2]);
	printf("div=%d\n",res[3]);

}





