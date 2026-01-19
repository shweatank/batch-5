#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int res;
int main() {
	int a,b;
	int status;
	printf("Enter the num1 and num2: ");
	scanf("%d %d",&a,&b);
//	int pipefd[2];
//	pipe(pipefd);
//	perror("Pipe\n");
	if(fork() != 0) {
		res = a+b;
		printf("In parent process\n");
		printf("Addition Result: %d\n",res);
		printf("%p\n",&res);
	}
	else {
		printf("In child process\n");
		printf("Sum result from parent process is: %d\n",res);
		res = a-b;
		printf("Subtraction in child process is: %d\n",res);
		printf("%p\n",&res);
		exit(res);
	}
	wait(&status);
	printf("In parent process and global variable value is: %d\n",res);
//	printf("Subtraction res from child process is: %d\n",status>>8);
}
