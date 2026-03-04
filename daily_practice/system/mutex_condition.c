#include<stdio.h>
#include<pthread.h>

int j=1;
pthread_mutex_t a;
pthread_cond_t flag;

void *odd(void *arg){
	puts("in thread 1");
	while(j<=10){
		pthread_mutex_lock(&a);
		while(j%2==0)
			pthread_cond_wait(&flag,&a);
	printf("in odd j is %d\n",j);
		j++;
	pthread_cond_signal(&flag);
	pthread_mutex_unlock(&a);
//	printf("%d\n",j);
	}
	return NULL;	
}

void *even(void *arg){
	puts("in thread 2");
	while(j<=10){
		pthread_mutex_lock(&a);
		while((j%2)!=0)
			pthread_cond_wait(&flag,&a);
	printf("in even %d\n",j);
		j++;
	pthread_cond_signal(&flag);
	pthread_mutex_unlock(&a);
//	printf("%d\n",j);
	}
	return NULL;
}

int main(){
	pthread_t p,q;
	int t1,t2;
	pthread_mutex_init(&a,NULL);
	pthread_cond_init(&flag,NULL);
	t1=pthread_create(&p,NULL,even,NULL);
	t2=pthread_create(&q,NULL,odd,NULL);
	pthread_join(p,NULL);
	pthread_join(q,NULL);
}
