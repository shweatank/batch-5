#include <stdio.h>
#include <unistd.h>

int main() {
	int a = 10;
	int b = 20;
	printf("%d\n",a+b);
	printf("%d , %d\n",getpid(),getppid());
	fork();
	fork();
	printf("%d\n",a+b);
	printf("%d , %d\n",getpid(),getppid());
	/*fork();
	printf("%d\n",a+b);
	printf("%d , %d\n",getpid(),getppid());*/
}

