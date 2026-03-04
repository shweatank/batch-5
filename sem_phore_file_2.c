#include<stdio.h>
#include<unistd.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#include<fcntl.h>

int main()
{
	int semid=semget(1045,2,IPC_CREAT|0664);
	if(semid<0)
	{
		perror("semget");
		return 0;
	}

	int fd=open("data",O_WRONLY|O_APPEND);
	if(fd<0)
	{
		perror("open");
		return 0;
	}


	struct sembuf v={1,0,0};

//	semctl(semid,1,SETVAL,1);
//	semctl(semid,0,SETVAL,1);
	char ch='A';

	for(ch;ch<='Z';ch++)
	{
		semop(semid,&v,1);
		semctl(semid,1,SETVAL,1);
		write(fd,&ch,sizeof(ch));
		semctl(semid,0,SETVAL,0);
	}

}
