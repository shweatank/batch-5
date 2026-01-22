#include<stdio.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<string.h>
#include<stdlib.h>
struct msgbuf
{
	long msgtyp;
	int res;
};
int main(int argc,char **argv)
{
	struct msgbuf m;
	int id;
	while(1)
	{
		id=msgget(1,IPC_CREAT|0666);
		msgrcv(id,&m,sizeof(m.res),atoi(argv[1]),0);
		printf("%d",m.res);
	}
}
