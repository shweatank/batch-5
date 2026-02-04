#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

#define SIZE	5

int buf[SIZE], in = 0, out = 0;
sem_t empty,full,mutex;

void* producer(void *args) {
	int item;
	for(item = 1;item <= 10;item++) {
		sem_wait(&empty);
		sem_wait(&mutex);

		buf[in] = item;
		in = (in + 1) % SIZE;
		printf("produced: %d\n",item);

		sem_post(&mutex);
		sem_post(&full);
	}
	return NULL;
}

void *consumer(void *args) {
	int item;
	for(item = 1;item <= 10;item++) {
		sem_wait(&full);
		sem_wait(&mutex);

		item = buf[out];
		printf("consumed = %d\n",item);
		out = (out + 1) % SIZE;

		sem_post(&mutex);
		sem_post(&empty);
	}
	return NULL;
}

int main() {
	pthread_t t1,t2;
	sem_init(&empty,0,SIZE);
	sem_init(&full,0,0);
	sem_init(&mutex,0,1);

	pthread_create(&t1,NULL,producer,NULL);
	pthread_create(&t2,NULL,consumer,NULL);

	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
}
