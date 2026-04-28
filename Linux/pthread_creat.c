#include<stdio.h>
#include<pthread.h>

pthread_cond_t flag;
pthread_mutex_t a;
int j=1;

void * even(void *)
{
	printf("in thread 1\n");
	while(j<=10)
	{
		pthread_mutex_lock(&a);
		while(j%2!=0)
			pthread_cond_wait(&flag,&a);
		printf("even is %d\n",j);
		j++;
		pthread_cond_signal(&flag);
		pthread_mutex_unlock(&a);
	}
	return NULL;

}

void * odd(void *)
{
	printf("in thread 2\n");
	while(j<=10)
	{
		pthread_mutex_lock(&a);
		while(j%2==0)
			pthread_cond_wait(&flag,&a);
		printf("odd is %d\n",j);
		j++;
		pthread_cond_signal(&flag);
		pthread_mutex_unlock(&a);
	}
	return NULL;
}

int main()
{
	int t1,t2;
	pthread_t p1=0,p2=0;
	pthread_mutex_init(&a,NULL);
	pthread_cond_init(&flag,NULL);
	t1=pthread_create(&p1,NULL,odd,NULL);
	t2=pthread_create(&p2,NULL,even,NULL);
	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
}


