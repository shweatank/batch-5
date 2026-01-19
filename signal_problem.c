#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>

/*
void sighandler(int status) {
	printf("In sighandler\n");
	wait(&status);
	printf("Return value from child is : %d\n",status>>8);
}


int main() {
	int a,b,sum,pid;
	printf("Enter the numbers: ");
	scanf("%d %d",&a,&b);
	pid = fork();
	if(pid == 0) {
		printf("In child process\n");
		sum = a+b;
		exit(sum);
	}
	else {
		signal(3,sighandler);
		while(1);
	}
}

*/

int main(int argc,char **argv) {
	char sum;
	if(read(atoi(argv[1]),&sum,sizeof(sum)) != 4) {
		perror("Fifo file descriptor is invalid\n");
		return 0;
	}
	printf("Sum: %d\n",sum);
}

