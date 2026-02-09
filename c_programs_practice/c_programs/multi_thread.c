#include<stdio.h>
#include<pthread.h>

pthread_t t1,t2;
pthread_mutex_t m1;

char buf[100];

void *thread1(void *p){
	while(1){
		pthread_mutex_lock(&m1);
		scanf("%s",buf);
		pthread_mutex_unlock(&m1);
		//pthread_exit(1);
		sleep(1);
	}
}

void *thread2(void *p){
	while(1){
	//	pthread_join(t1,NULL);
		pthread_mutex_lock(&m1);
		printf("%s\n",buf);
		pthread_mutex_unlock(&m1);
		sleep(1);
	}
	
}

int main(){
	pthread_mutex_init(&m1,NULL);
	pthread_create(&t1,NULL,thread1,NULL);
	pthread_create(&t2,NULL,thread2,NULL);

	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
}




