#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
	char *ptr = shmat(0,0,0);
	printf("%s\n",ptr);
	sleep(10);
	shmdt(ptr);
	sleep(5);
	shmctl(0,IPC_RMID,NULL);
}
