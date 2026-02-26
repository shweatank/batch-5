#include<stdio.h>
#include<pthread.h>

pthread_mutex_t m=PTHREAD_MUTEX_INITIALIZER;

int x=0;

void * thread1(void *p)
{
while(1)
{
pthread_mutex_lock(&m);
if(x==0)
printf("%c\n",97);
x=1;
pthread_mutex_unlock(&m);
sleep(1);
}
}
void * thread2(void *p)
{
while(1)
{
pthread_mutex_lock(&m);

printf("%c\n",65);
pthread_mutex_unlock(&m);
sleep(1);
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

