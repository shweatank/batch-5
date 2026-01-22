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
	int a,b;
	while(1)
	{
		int key=ftok("file",65);
		int msgid=msgget(key,0666);
		printf("enter the a and b\n");
		scanf("%d %d",&a,&b);
		struct msg x;
		x.key1=1;
		x.res=a+b;
		msgsnd(msgid,&x,sizeof(x),0);

	}
}
