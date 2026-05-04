#include<stdio.h>
#include<pthread.h>
pthread_mutex_t m=PTHREAD_MUTEX_INITIALIZER;
int num=0;
int max=10;
void * even_thread(void *p)
{
while(num<=max)
{
pthread_mutex_lock(&m);
if(num%2==0)
printf("%d\n",num++);

pthread_mutex_unlock(&m);
}
}

void * odd_thread(void *p)
{
while(num<=max)
{
pthread_mutex_lock(&m);
if(num%2!=0)
printf("%d\n",num++);
pthread_mutex_unlock(&m);
}
}

void main()
{
pthread_t t1,t2;

pthread_create(&t1,0,even_thread,0);
pthread_create(&t2,0,odd_thread,0);
pthread_join(t1,0);
pthread_join(t2,0);
}
