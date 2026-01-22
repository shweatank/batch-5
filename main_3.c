#include<stdio.h>
#include<sys/msg.h>
#include<sys/ipc.h>
#include<signal.h>
#include<sys/wait.h>
#include<unistd.h>
void handler()
{
	printf("in handler\n");
}
struct msgbuf
{
	long msgtype;
	int res;
};
int main()
{
	struct msgbuf m;
	int id;
	while(1)
	{
		//printf("addition from another process is %d\n",m.res);
		id=msgget(1,IPC_CREAT|0666);
		msgrcv(id,&m,sizeof(m.res),1,0);
		signal(SIGUSR1,handler);
		pause();
		printf("addition from another process is %d\n",m.res);

	}
}
