#include<stdio.h>
#include<pthread.h>


int j=1;
pthread_mutex_t mutex;
pthread_cond_t flag;

void *odd(void *arg)
{
	puts("in thread 1");
	while(j<=10)
	{
		while(j%2==0)
			pthread_cond_wait(&flag,&mutex);
		printf("odd is %d\n",j);
		j++;
		pthread_cond_signal(&flag);
		pthread_mutex_unlock(&mutex);
	}
}

void *even(void *arg)
{
	puts("in thread 2");
	while(j<=10)
	{
		while(j%2!=0)
			pthread_cond_wait(&flag,&mutex);
		printf("even is %d\n",j);
		j++;
		pthread_cond_signal(&flag);
		pthread_mutex_unlock(&mutex);
	}
}
int main()
{
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&flag,NULL);
	pthread_t t1,t2;
	pthread_create(&t1,NULL,even,NULL);
	pthread_create(&t2,NULL,odd,NULL);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
}
