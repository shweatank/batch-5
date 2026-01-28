#include"common.h"
struct thread_data
{
	int num1;
	int num2;
};

void* addition(void* arg)
{
	struct thread_data *d=(struct thread_data*)arg;
	struct msgbuf_res v;
	v.mtype=1;
	v.op=1;
	v.val=d->num1+d->num2;
	int id=msgget(1,IPC_CREAT|0664);
	msgsnd(id,&v,sizeof(v)-sizeof(long),0);
	perror("msgsnd");
	pthread_exit(NULL);
}

void *subtraction(void* arg)
{
	struct thread_data *d=(struct thread_data*)arg;
	struct msgbuf_res v;
	v.mtype=1;
	v.op=2;
	v.val=d->num1-d->num2;
	int id=msgget(1,IPC_CREAT|0664);
	msgsnd(id,&v,sizeof(v)-sizeof(long),0);
	perror("msgsnd");
	pthread_exit(NULL);
}

void *multiplication(void* arg)
{
	struct thread_data *d=(struct thread_data*)arg;
	struct msgbuf_res v;
	v.mtype=1;
	v.op=3;
	v.val=d->num1*d->num2;
	int id=msgget(1,IPC_CREAT|0664);
	msgsnd(id,&v,sizeof(v)-sizeof(long),0);
	perror("msgsnd");
	pthread_exit(NULL);
}

void *division(void* arg)
{
	struct thread_data *d=(struct thread_data*)arg;
	struct msgbuf_res v;
	v.mtype=1;
	v.op=4;
	v.val=d->num1/d->num2;
	int id=msgget(1,IPC_CREAT|0664);
	msgsnd(id,&v,sizeof(v)-sizeof(long),0);
	perror("msgsnd");
	pthread_exit(NULL);
}

void main()
{
	pthread_t t1,t2,t3,t4;
	struct thread_data d;
	d.num1=20;
	d.num2=10;

	pthread_create(&t1,NULL,addition,(void*)&d);
	pthread_create(&t2,NULL,subtraction,(void*)&d);
	pthread_create(&t3,NULL,multiplication,(void*)&d);
	pthread_create(&t4,NULL,division,(void*)&d);

	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	pthread_join(t3,NULL);
	pthread_join(t4,NULL);
}
