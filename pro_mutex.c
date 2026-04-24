#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<fcntl.h>
#include<unistd.h>
int a[5];
pthread_mutex_t lock;
pthread_cond_t cond;
int flag;
void * producer(void*p)
{
int i;
while(1)
{
pthread_mutex_lock(&lock);
while(flag==1)
{
printf("consumer reading the data\n");
pthread_cond_wait(&cond,&lock);
}
for(i=0;i<5;i++)
{
a[i]=i+10;
printf("producer write data=%d\n",a[i]);
sleep(1);
}
flag=1;
pthread_cond_signal(&cond);
pthread_mutex_unlock(&lock);
}
}
void *consumer(void*p)
{
int i;
while(1)
{
pthread_mutex_lock(&lock);
while(flag==0)
{
printf("producer producing the data\n");
pthread_cond_wait(&cond,&lock);
}
for(i=0;i<5;i++)
{
printf("consumer reading=%d\n",a[i]);
sleep(1);
}
flag=0;
pthread_cond_signal(&cond);
pthread_mutex_unlock(&lock);
}
}
void main()
{
pthread_t t1,t2;
pthread_create(&t1,0,producer,NULL);
pthread_create(&t2,0,consumer,NULL);
pthread_join(t1,NULL);
pthread_join(t2,NULL);
}
