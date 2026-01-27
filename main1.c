#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include"header.h"
#include<string.h>
int main(int argc,char*a[])
{
	int res;
	if(argc!=2)
	{
		printf("exe mtype\n");
		return 0;
	}
	int msgid=msgget(5,IPC_CREAT|0664);
	if(msgid<0)
	{
		perror("msgget");
		return 0;
	}
	printf("%d\n",msgid);
	msgrcv(msgid,&v,sizeof(int),atoi(a[1]),0);
	switch(v.mtype)
	{
		case 1:printf("addition:%d\n",v.data);break;
		case 2:printf("subtraction:%d\n",v.data);break;
		case 3:printf("multiplition:%d\n",v.data);break;
		case 4:printf("division:%d\n",v.data);break;
		default:printf("invalid\n");
	}
}
