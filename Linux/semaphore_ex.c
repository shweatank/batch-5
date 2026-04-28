#include<stdio.h>
#include<unistd.h>
#include<sys/sem.h>
#include<fcntl.h>

int main()
{
	struct sembuf var={1,0,0};
	int fd=open("sembuf",O_CREAT|O_RDWR,0666);
	if(fd<0)
	{
		perror("open");
		return 0;
	}
	int id=semget(3,2,IPC_CREAT|0664);
	if(id<0)
	{
		perror("semget");
		return 0;
	}
	semop(id,&var,1);
	semctl(id,0,SETVAL,1);
	for(char ch='a';ch<='z';ch++)
		write(fd,&ch,1);
	semctl(id,0,SETVAL,0);
	return 0;
}
