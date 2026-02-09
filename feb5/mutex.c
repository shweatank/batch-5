#include<stdio.h>
#include<pthread.h>

pthread_mutex_t lock;
static int cnt; 
void* cs(void *arg)
{
	pthread_mutex_lock(&lock);
	cnt++;
	pthread_mutex_unlock(&lock);
}

int main()
{
	pthread_t td1,td2;

	pthread_mutex_init(&lock,NULL);
	pthread_create(&td1,NULL,cs,NULL);
	pthread_create(&td2,NULL,cs,NULL);

	pthread_join(td1,NULL);
	pthread_join(td2,NULL);

	printf("data : %d\n",cnt);
	
	pthread_mutex_destroy(&lock);
	return 0;
}
