#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
int main()
{
	int a,b;
	int res;
	while(1)
	{
		int key=ftok("file",65);
		int msgid=msgget(key,0666);
		printf("enter the a and b\n");
		scanf("%d %d",&a,&b);
		res=a/b;
		msgsnd(msgid,&res,sizeof(res),0);

	}
}
