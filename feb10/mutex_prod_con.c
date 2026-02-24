#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

pthread_mutex_t lock;
int val;

void* producer(void *)
{
	pthread_mutex_lock(&lock);
	val=100;
	printf("value produced : %d\n",val);
	pthread_mutex_unlock(&lock);
	return NULL;
}


void* consumer(void *)
{
	usleep(1);
	pthread_mutex_lock(&lock);
	printf("value consumed : %d\n",val);
	pthread_mutex_unlock(&lock);
	return NULL;
}

int main()
{
	pthread_t td1,td2;

	pthread_create(&td1,NULL,producer,NULL);
	pthread_create(&td2,NULL,consumer,NULL);

	pthread_join(td1,NULL);
	pthread_join(td2,NULL);

	pthread_mutex_destroy(&lock);
}
