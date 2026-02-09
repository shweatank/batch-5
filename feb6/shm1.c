#include<stdio.h>
#include<sys/shm.h>
#include<sys/ipc.h>
int arr[10];

int main(int argc,char **argv)
{
	int shmid=shmget(1,10,IPC_CREAT|0666);
	if(shmid<0)
	{
		perror("shmget");
		return 0;
	}
	char str[20],*ptr=(char*)shmat(shmid,0,0);
	printf("enter share mem data : ");
	scanf("%s",ptr);
	printf("data: %s",ptr);

	shmdt(ptr);
	shmctl(shmid,IPC_RMID,&ptr);
	perror("shmctl");
}
