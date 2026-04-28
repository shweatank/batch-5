#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/sem.h>

int main()
{
	struct sembuf var={1,0,0};
	int fd=open("sembuf",O_CREAT|O_WRONLY|O_APPEND,0666);
	perror("open");
	int id;
	char ch;
	id=semget(2,3,IPC_CREAT|0664);
	if(id<0)
	{
		perror("semget");
	}
//	semctl(id,1,SETVAL,1);
//semctl(id,2,SETVAL,0);
	for(ch='a';ch<='z';ch++)
	{
	      semop(id,&var,1);
	//	semctl(id,1,SETVAL,1);
		write(fd,&ch,1);
		printf("%c written\n",ch);
		semctl(id,1,SETVAL,1);
		semctl(id,2,SETVAL,0);
	}
return 0;
}
