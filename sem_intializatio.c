
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#include<sys/shm.h>

int main()
{

	int id=semget(100,5,IPC_CREAT|0664);
	if(id<0)
	{
		perror("semget");
		return 0;
	}


         struct sembuf v;

    
	 v.sem_num=0;
         v.sem_op=0;
	 v.sem_flg=0;

   
	semctl(id,0,SETVAL,0);
	semctl(id,1,SETVAL,1);
	semctl(id,2,SETVAL,1);
	semctl(id,3,SETVAL,1);
	semctl(id,4,SETVAL,1);

}

