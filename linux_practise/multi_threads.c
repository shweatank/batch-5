#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

int count = 0;

void* increment(void *args) {
	for(int i = 0;i<100;i++) {
		count++;
	}
	return NULL;
}

int main() {
	pthread_t t1,t2;
	pthread_create(&t1,NULL,increment,NULL);
	pthread_create(&t2,NULL,increment,NULL);

	pthread_join(t1,NULL);
	printf("count = %d\n",count);
	pthread_join(t2,NULL);

	printf("count = %d\n",count);
}
