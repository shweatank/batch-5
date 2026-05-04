#include<stdio.h>
#include<pthread.h>
pthread_mutex_t m=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c=PTHREAD_COND_INITIALIZER;
int num=0;
int max=10;
void *even(void *p)
{
pthread_mutex_lock(&m);

while(num%2==0)
pthread_cond_wait(&c,&m);

if(num<=max)
printf("%d\n",num++);

pthread_cond_signal(&c);
pthread_mutex_unlock(&m);
}

void *odd(void *p)
{
while(num<=max)
{
pthread_mutex_lock(&m);

while(num%2!=0)
pthread_cond_wait(&c,&m);

if(num<=max)
printf("%d\n",num++);

pthread_cond_signal(&c);
pthread_mutex_unlock(&m);
}
int main()
{
pthread_t t1,t2;

pthread_create(&t1,0,even,0);
pthread_create(&t2,0,odd,0);

pthread_join(t1,0);
pthread_join(t2,0);
}
