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
	if(argc!=3)
	{
		printf("exe mtype msg_or_data\n");
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
        v.mtype=atoi(argv[1]);
	strcpy(v.data,argv[2]);       
	msgsnd(msgid,&v,sizeof(v.data),0);
	perror("msgsnd");	
		
}
