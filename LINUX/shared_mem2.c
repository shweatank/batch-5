#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/shm.h>
int main()
{
	key_t key=5;
	size_t size=50;
	int id=shmget(key,size,IPC_CREAT|0664);
	if(id<0){perror("shmget");return 0;}
	printf("id=%d\n",id);
	char *p=shmat(id,0,0);
	printf("%s\n",p);
	shmdt(p);
	shmctl(id,IPC_RMID,0);
	return 0;
	
}
