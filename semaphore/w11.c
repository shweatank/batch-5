#include"header.h"
void main()
{
int id=semget(3,5,IPC_CREAT|0664);
	if(id<0)
	{
	perror("semget");
	return;
	}
printf("id=%d\n",id);
/////////////////////////////////////
int fd=open("DATA",O_WRONLY|O_APPEND|O_CREAT,0664);
	if(fd<0)
	{
	perror("open");
	return;
	}
printf("fd=%d\n",fd);
///////////////////////////////////
char ch;
struct sembuf v;
v.sem_num=2;
v.sem_op=0;
v.sem_flg=0;
	for(ch='a';ch<='z';ch++)
	{
	semop(id,&v,1);
	semctl(id,3,SETVAL,1);
	write(fd,&ch,1);
	semctl(id,3,SETVAL,0);
	semctl(id,2,SETVAL,1);
	}
	printf("Done...\n");
}
