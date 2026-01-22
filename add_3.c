#include<stdio.h>
#include<sys/msg.h>
#include<sys/ipc.h>
struct msgbuf
{
	long msgtype;
	int res;
};
int main()
{
	int a,b;
	int id;
	struct msgbuf m;
	m.msgtype=1;
	while(1)
	{
		id=msgget(1,IPC_CREAT|0666);
		printf("enter the a and b\n");
		scanf("%d %d",&a,&b);
		m.res=a+b;
		msgsnd(id,&m,sizeof(m.res),0);
	}

}
