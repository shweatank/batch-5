#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<pthread.h>

pthread_t tid1,tid2;
sem_t s1,s2;

void*t1(void *p){
	char ch='a';
	while(ch<='z'){
		sem_wait(&s1);
		printf("%c ",ch);
		ch++;
		sem_post(&s2);
	}
}
void*t2(void *p){
	char ch='A';
	while(ch<='Z'){
		sem_wait(&s2);
		printf("%c ",ch);
		ch++;
		sem_post(&s1);
	}
}


int main()
{
	sem_init(&s1,0,1);
	sem_init(&s2,0,0);
	pthread_create(&tid1,NULL,t1,NULL);
	pthread_create(&tid2,NULL,t2,NULL);
	pthread_join(tid1,NULL);
	printf("\n");
}


