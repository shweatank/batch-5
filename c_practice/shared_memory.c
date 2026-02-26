#include<stdio.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	int key=1,size=100;
	int shmid=shmget(key,size,IPC_CREAT | 0666);
	if(shmid<0)
	{
		perror("shmget");
		exit(0);
	}
	
	char *shm_mem=(char *)shmat(shmid,NULL,0);
	strcpy(shm_mem,"hello mirafra");
	printf("data written\n");
	getchar();
	shmdt(shm_mem);
	return 0;
}
