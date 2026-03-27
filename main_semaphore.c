
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#include<sys/shm.h>

int main()
{
      
	int sd=shmget(78,1024,IPC_CREAT|0664);
	if(sd<0)
	{
		perror("shmget");
		return 0;
	}

	int id=semget(100,5,IPC_CREAT|0664);
	if(id<0)
	{
		perror("semget");
		return 0;
	}

	int c=0;

         struct sembuf v;

    
	 v.sem_num=4;
         v.sem_op=0;
	 v.sem_flg=0;


for(int i=1;i<5;i++)
{	

        int *p=(int *)shmat(sd,NULL,0);

        semop(id,&v,1);    // the function which decicdes weather to enter  or not;
	{
	printf("result=%d\n",*p);
        shmdt(p);                              // critical section of code which will be controlled by the semaphore
	semctl(id,4,SETVAL,1);
	semctl(id,i,SETVAL,0);
	}
  }
}

