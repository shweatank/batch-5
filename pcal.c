#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/msg.h>
#include<sys/ipc.h>
#include<stdlib.h>
pthread_t ad,s,m,d;
int id,a[2];
struct msgbuff
{
	long type;
	int data;
}v;
void *add(void *ar)
{
	v.type=1;
	v.data=a[0]+a[1];
	msgsnd(id,&v,sizeof(int),0);
	pthread_exit(0);
		
}
void *sub(void *ar)
{
        v.type=2;
        v.data=a[0]-a[1];
        msgsnd(id,&v,sizeof(int),0);
        pthread_exit(0);

}
void *mul(void *ar)
{
        v.type=3;
        v.data=a[0]*a[1];
        msgsnd(id,&v,sizeof(int),0);
        pthread_exit(0);

}
void *divi(void *ar)
{
        v.type=4;
        v.data=a[0]/a[1];
        msgsnd(id,&v,sizeof(int),0);
        pthread_exit(0);

}

int main(int argc,char *argv[])
{
	if(argc<3){printf("a.out a,b\n");return 0;}
	id=msgget(7,IPC_CREAT|0664);
	a[0]=atoi(argv[1]);
	a[1]=atoi(argv[2]);
	pthread_create(&ad,0,add,0);
	pthread_create(&s,0,sub,0);
	pthread_create(&m,0,mul,0);
	pthread_create(&d,0,divi,0);
	pthread_join(ad,NULL);
	pthread_join(s,NULL);
	pthread_join(m,NULL);
	pthread_join(d,NULL);
	pthread_exit(0);
}
