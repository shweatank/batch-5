#include"header.h"
pthread_mutex_t lock;
int a,b,res[4];

void* add(void *p){
	pthread_mutex_lock(&lock);
	res[0]=a+b;
	pthread_mutex_unlock(&lock);
	pthread_exit(NULL);
}
void* sub(void*p){
	pthread_mutex_lock(&lock);
	res[1]=a-b;
	pthread_mutex_unlock(&lock);
	pthread_exit(NULL);
}

void* mul(void*p){
	pthread_mutex_lock(&lock);
	res[2]=a*b;
	pthread_mutex_unlock(&lock);
	pthread_exit(NULL);
}

void* divi(void*p){
	pthread_mutex_lock(&lock);
	res[3]=a/b;
	pthread_mutex_unlock(&lock);
	pthread_exit(NULL);
}

void main(){
printf("Enter a and b values\n");
scanf("%d%d",&a,&b);
	pthread_t t1,t2,t3,t4;
	pthread_mutex_init(&lock,NULL);
	pthread_create(&t1,0,add,0);
	pthread_create(&t2,0,sub,0);
	pthread_create(&t3,0,mul,0);
	pthread_create(&t4,0,divi,0);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	pthread_join(t3,NULL);
	pthread_join(t4,NULL);
//	sleep(1);
	printf("add=%d\n",res[0]);
	printf("sub=%d\n",res[1]);
	printf("mul=%d\n",res[2]);
	printf("div=%d\n",res[3]);
	pthread_mutex_destroy(&lock);
}
