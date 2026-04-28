#include<stdio.h>
#include<unistd.h>
#include<sys/sem.h>
#include<fcntl.h>

int main()
{
	char ch;
	struct sembuf var={2,0,0};
	int id,fd=open("sembuf",O_CREAT|O_WRONLY|O_APPEND,0666);
	if(fd<0)
		perror("open");
	   id=semget(2,3,IPC_CREAT|0664);
	if(id<0)
	{
		perror("semget");
	}
	semctl(id,1,SETVAL,1);
	semctl(id,2,SETVAL,0);
	for(ch='A';ch<='Z';ch++)
	{
		semop(id,&var,1);
		//	semctl(id,2,SETVAL,1);
		//			write(fd,&ch,1);
		printf("%c write\n",ch);
		semctl(id,2,SETVAL,1);
		semctl(id,1,SETVAL,0);
	}
	return 0;

}
