#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<semaphore.h>
sem_t *sem_prod,*sem_cons;
int main()
{
	int fd;
	sem_prod=sem_open("my_sem_prod",0);
	sem_cons=sem_open("my_sem_cons",0);
       	fd=shmget(1,240,0666);
	if(fd<0)
	{
		perror("shmget");
		return 0;
	}
	int *p=shmat(fd,NULL,0);
	int j=0;
	sem_wait(sem_cons);
	for(int i=0;i<60;i++)
	{
		if(p[i]%2==0)
			p[j++]=p[i];
	}
	for(int i=j;i<60;i++)
		p[i]=0;
	sem_post(sem_prod);
	puts("");
	shmdt(p);
}
