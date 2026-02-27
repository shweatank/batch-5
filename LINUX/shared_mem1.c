#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/shm.h>
int main()	
{
	key_t key=5;
	size_t size=50;
	//key=ftok("pras",0);
	int id=shmget(key,size,IPC_CREAT|0664);
	if(id<0){perror("shmget");return 0;}
	printf("id=%d\n",id);
	char *p=NULL;
	p=shmat(id,0,0);
	printf("enter a string:");scanf("%s",p);
	shmdt(p);	
	return 0;
}
