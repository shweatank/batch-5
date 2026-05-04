#include<stdio.h>
#include<pthread.h>


pthread_mutex_t m=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c=PTHREAD_COND_INITIALIZER;

int x=0;

void * thread1(void *p)
{
while(1)
{
pthread_mutex_lock(&m);

while(x!=0)
pthread_cond_wait(&c,&m);

printf("A\n");

x=1;
pthread_cond_signal(&c);

pthread_mutex_unlock(&m);


}


}


void * thread2(void *p)
{


while(1)
{
pthread_mutex_lock(&m);

while(x!=1)
pthread_cond_wait(&c,&m);

printf("B\n");

x=0;
pthread_cond_signal(&c);

pthread_mutex_unlock(&m);


}

}


int main()
{
pthread_t t1,t2;

pthread_create(&t1,0,thread1,0);
pthread_create(&t2,0,thread2,0);

pthread_join(t1,0);
pthread_join(t2,0);
return 0;
}
