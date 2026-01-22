#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
int main()
{
	struct msg
	{
		int key1;
		int res;
	};
	while(1)
	{
		int i;
		int res;
		int key=ftok("file",65);
		struct msg x;
		int msgid=msgget(key,0666);
		msgrcv(msgid,&x,sizeof(res),i,0);
		if(x.key1==1)
		printf("%d",x.res);
	}	
}
