#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/ipc.h>
int main()
{
	int id=shmget(9,20,IPC_CREAT|0664);
	if(id<0)
	{
		perror("shmget");
		return 0;
	}
	printf("id:%d\n",id);
	int *ptr=shmat(id,0,0);
	printf("%d",*ptr);
	shmdt(ptr);
}
