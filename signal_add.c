#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/msg.h>

void sighandler(int dummy) {
	int a,b,sum;
	printf("Enter the numbers: ");
	scanf("%d %d",&a,&b);
	sum = a+b;
	int id = msgget(1,IPC_CREAT);
	perror("Msg created\n");
	msgsnd(id,&sum,4,NULL);
}

int main() {
	signal(3,sighandler);
	while(1);
}

