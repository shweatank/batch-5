//two threads thread1 printing even no and thread2 printing odd no 
//sync is done using mutex


#include<stdio.h>
#include<pthread.h>

pthread_mutex_t m=PTHREAD_MUTEX_INITIALIZER;
int num=0;
int max=10;

void * thread1(void *p)
{
while(num<=max)
{
pthread_mutex_lock(&m);
if(num%2==0)
printf("%d\n",num++);
pthread_mutex_unlock(&m);
}
}


void * thread2(void *p)
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

pthread_create(&t1,0,thread1,0);
pthread_create(&t2,0,thread2,0);

pthread_join(t1,0);
pthread_join(t2,0);
}


