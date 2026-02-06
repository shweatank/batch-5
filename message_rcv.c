#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/msg.h>
#include<sys/ipc.h>
struct mymsg
{
	long mtype;
	char data[20];
}v; 
int main(int argc,char *argv[])
{
	if(argc!=2)
	{
		printf("exe mtype\n");
		return 0;
	}
	key_t key=5;
	int msgid=msgget(key,IPC_CREAT|0666);
	if(msgid<0)
	{
		perror("msgget");
		return 0;
	}
	printf("msgid:%d\n",msgid);
	msgrcv(msgid,&v,sizeof(v.data),atoi(argv[1]),0);
	//msgrcv(msgid,&v,sizeof(v.data),0,0);  //kernel will give first msg irrespective of type
	//msgrcv(msgid,&v,sizeof(v.data),atoi(argv[1]),IPC_NOWAIT); //no message of desire type present rcv process terminates
	//printf("msg type:%ld\n",atoi(argv[1]));
	puts(v.data);
}

/*
./rcv 2
----------
./rcv 0
msgid:2
msg type:0
hhh
*/

/* //number greter than types......gives the lowest msgtype 1st msg
./rcv 4
msgid:2
msg type:4
man
*/

/* 
./rcv -5
1st msg with lowest type*/
