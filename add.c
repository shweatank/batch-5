#include <stdio.h>
#include <sys/shm.h>

int add(int num1,int num2) {
	return num1+num2;
}

int main() {
	int num1,num2,res;
	int *addr;
	printf("Enter the numbers: ");
	scanf("%d %d",&num1,&num2);
	res = add(num1,num2);
	int id = shmget(1,4,IPC_CREAT|IPC_EXCL|0666);
	perror("shmget\n");
	addr = shmat(id,0,0);
	*addr = res;
}

