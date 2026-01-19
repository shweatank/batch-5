#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/msg.h>
#include <stdlib.h>

int a,b;
pthread_t pid1,pid2,pid3,pid4;
struct msgbuf
{
	long mtype;
	int res;
};

void msgfunc(int sum) {
	struct msgbuf v;
        v.mtype=1;
        v.res = sum;
        int id;
        id=msgget(1,IPC_CREAT|0660);
        if(id<0)
        {
                perror("msgget");
                return;
        }
        if(msgsnd(id,&v,sizeof(v.res),0)==-1)
        {
                        perror("msgsnd");
                        exit(0);
        }
        printf("Successfully Message is written into MQ\n");
}

void *add(void *dummy) {
	int sum = a+b;
	printf("Sum is: %d\n",sum);
	msgfunc(sum);
	return NULL;
}

void *sub(void *dummy) {
	int sub = a-b;
        printf("Sub is: %d\n",sub);
	msgfunc(sub);
        return NULL;
}

void *mul(void *dummy) {
	int mul = a*b;
        printf("Mul is: %d\n",mul);
	msgfunc(mul);
        return NULL;
}

void *division(void *dummy) {
	int div = a/b;
        printf("Div is: %d\n",div);
	msgfunc(div);
        return NULL;
}

int main() {
	printf("Enter the numbers: ");
	scanf("%d %d",&a,&b);
	pthread_create(&pid1,NULL,add,NULL);
	pthread_create(&pid2,NULL,sub,NULL);
	pthread_create(&pid3,NULL,mul,NULL);
	pthread_create(&pid4,NULL,division,NULL);
	pthread_join(pid1,NULL);
	pthread_join(pid2,NULL);
	pthread_join(pid3,NULL);
	pthread_join(pid4,NULL);
	pthread_exit(NULL);
}
