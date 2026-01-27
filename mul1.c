#include<stdio.h>
#include<sys/msg.h>
#include<sys/ipc.h>
#include"header.h"
int main(int argc,char *a[])
{
	int a1,b,r;
	if(argc!=4)
        {
                printf("exe mtype a b\n");
                return 0;
        }
	a1=atoi(a[2]);
	b=atoi(a[3]);
	r=a1*b;
        key_t key=5;
        int msgid=msgget(key,IPC_CREAT|0666);
        if(msgid<0)
        {
                perror("msgget");
                return 0;
        }
        printf("msgid:%d\n",msgid);
        v.mtype=atoi(a[1]);
        v.data=r;
	msgsnd(msgid,&v,sizeof(int),0);
        perror("msgsnd");


	
}
