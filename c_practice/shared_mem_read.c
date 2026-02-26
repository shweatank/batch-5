#include<stdio.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<stdlib.h>


int main()
{
	int shmid=shmget(1,100,0666);
	if(shmid<0)
	{
		perror("shmget");
		return 0;
	}

	char *ptr=(char *)shmat(shmid,NULL,0);
	printf("data : %s\n",ptr);
	shmdt(ptr);
	return 0;
}
