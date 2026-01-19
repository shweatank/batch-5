#include"header.h"
struct msg
{
	long mtype;
	char data[20];
}v;
void main()
{
	int id;
	id=msgget(1,IPC_CREAT|0664);
	if(id<0)
	{
		perror("msgget");
		return;
	}
	if(fork()==0)
	{
		while(1)
		{
			msgrcv(id,&v,sizeof(v.data),3,0);
			printf(" recieving data from mtype 3=%s\n",v.data);
		}
	}
	else
	{
		while(1)
		{
			printf("in par enter string for mtype 2\n");
			scanf("%s",v.data);
			v.mtype=2;
			int ret=msgsnd(id,&v,strlen(v.data)+1,0);
			if(ret<0)
			{
				perror("msgsnd");
				return;
			}
		}
	}
}

