#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

int count = 0;
sem_t sem;

void* increment(void *args) {
	for(int i = 0;i<100;i++) {
		sem_wait(&sem);
		count++;
		sem_post(&sem);
	}
	return NULL;
}

int main() {
	pthread_t t1,t2;
	sem_init(&sem,0,1);
	pthread_create(&t1,NULL,increment,NULL);	
	pthread_create(&t2,NULL,increment,NULL);

	pthread_join(t1,NULL);
	printf("count = %d\n",count);
	pthread_join(t2,NULL);

	printf("count = %d\n",count);
}
