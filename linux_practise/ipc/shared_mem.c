#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>

int main() {
	int shmid = shmget(1,100,IPC_CREAT|0666);
	perror("shmget");
	printf("%d\n",shmid);
	char *ptr = shmat(shmid,0,0);
	printf("Enter the data for shared memory: ");
	scanf("%s",ptr);
	
}
