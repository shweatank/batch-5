#include<stdio.h>
#include<math.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>

pthread_mutex_t lock;
int arr[10];

int check_prime(int num)
{
	int sq;
	if(num<2)
		return 0;
	sq=sqrt(num);
	for(int i=2;i<=sq;i++)
	{
		if(num%i==0)
			return 0;
	}
	return 1;
}

void print_array(void)
{
	for(int i=0;i<10;i++)
	{
		printf("%d ",arr[i]);
	}
	printf("\n");
}

void* thread1(void *arg)
{
	printf("in thread1\n");
	pthread_mutex_lock(&lock);
	for(int i=0;i<10;i++)
	{
		arr[i]=rand()%100;
	}
	print_array();
	pthread_mutex_unlock(&lock);
}

void* thread2(void *arg)
{
	printf("in thread2\n");
	pthread_mutex_lock(&lock);
	for(int i=0;i<10;i++)
	{
		if(check_prime(arr[i]))
			printf("%d ",arr[i]);
	}
	printf("\n");
	pthread_mutex_unlock(&lock);
}

int main()
{
	pthread_t td1,td2;

	pthread_mutex_init(&lock,NULL);

	pthread_create(&td1,NULL,thread1,NULL);
	pthread_create(&td2,NULL,thread2,NULL);

	pthread_join(td1,NULL);
	pthread_join(td2,NULL);

	pthread_mutex_destroy(&lock);
}

