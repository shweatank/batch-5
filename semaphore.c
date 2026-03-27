#include <stdio.h>
#include <pthread.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <unistd.h>

int semid;

void* thread1(void* arg)
{

   struct sembuf op={0,0,0};
    while(1)
    {
        //struct sembuf op={0,0,0};
	semop(semid,&op,1);

        printf("Thread 1\n");
	sleep(1);
	semctl(semid,0,SETVAL,1);
	semctl(semid,1,SETVAL,0);
    }
}

void* thread2(void* arg)
{

     struct sembuf op={1,0,0};
    while(1)
    {
	  //  struct sembuf op={1,0,0};
	    semop(semid,&op,1);
            printf("Thread 2\n");
	    sleep(1);
	    semctl(semid,1,SETVAL,1);
	    semctl(semid,2,SETVAL,0);            
    }
}

void* thread3(void* arg)
{

    struct sembuf op={2,0,0};
    while(1)
    {
       // struct sembuf op={2,0,0};
	semop(semid,&op,1);
        printf("Thread 3\n");
	sleep(1);
        semctl(semid,2,SETVAL,1);
	semctl(semid,0,SETVAL,0);
        
    }
}

int main()
{
    pthread_t t1, t2, t3;

    // Create semaphore set with 3 semaphores
    semid = semget(1239, 3, IPC_CREAT | 0666);

    // Initialize semaphore values
    semctl(semid, 0, SETVAL, 0);  // Thread1 starts
    semctl(semid, 1, SETVAL, 1);
    semctl(semid, 2, SETVAL, 1);

    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);
    pthread_create(&t3, NULL, thread3, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    return 0;
}
