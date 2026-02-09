#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

char buffer[5];
pthread_mutex_t lock;
int cnt;

void* producer(void* arg)
{
	int i;
	for(i=0;i<10;i++)
	{
		pthread_mutex_lock(&lock);
		if(i<5)
		{
			cnt++;
			buffer[i]='A'+i;
			printf("produced : %c\n",buffer[i]);
		}
		pthread_mutex_lock(&lock);
		sleep(1);
	}
}

void* consumer(void* arg)
{
	int i;
	for(i=0;i<10;i++)
	{
		pthread_mutex_lock(&lock);
		if(cnt>0)
		{
			printf("consumed : %c\n",buffer[i]);
			cnt--;
		}
		pthread_mutex_unlock(&lock);
		sleep(1);
	}
}

int main()
{
	pthread_t td1,td2;
	pthread_mutex_init(&lock,NULL);

	pthread_create(&td1,NULL,producer,NULL);
	pthread_create(&td2,NULL,consumer,NULL);

	pthread_join(td1,NULL);
	pthread_join(td2,NULL);
	
	pthread_mutex_destroy(&lock);
}
