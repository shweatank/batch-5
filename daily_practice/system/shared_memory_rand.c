#include<stdio.h>
#include<semaphore.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<stdlib.h>
#include<fcntl.h>
int main()
{
	int fd,arr[60];
	fd=shmget(1,240,IPC_CREAT|0666);
	if(fd<0)
	{
		perror("shmget");
		return 0;
	}
	sem_t *sem_prod=sem_open("my_sem_prod",O_CREAT,0666,1);
	sem_t *sem_cons=sem_open("my_sem_cons",O_CREAT,0666,0);
	int *p=shmat(fd,NULL,0);
	sem_wait(sem_prod);
	for(int i=0;i<60;i++)
	{
		p[i]=rand()%1000+1;
	}
	sem_post(sem_cons);
	for(int i=0;i<60;i++)
	{
		printf("%d ",p[i]);
	}
	sem_wait(sem_prod);
	shmdt(p);
}
