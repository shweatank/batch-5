#include"header.h"
struct sembuf p;
int ready;
pthread_mutex_t lock;
pthread_cond_t cond;
void*thread2_write(void*v)
{
pthread_mutex_lock(&lock);
for(int i='a';i<='z';i++)
printf("%c",i);
printf("\n");
ready=1;
pthread_cond_signal(&cond);
pthread_mutex_unlock(&lock);
pthread_exit(0);
}
void*thread1_write(void*v)
{
pthread_mutex_lock(&lock);
while(ready==0)
{
printf("waiting for signal\n");
pthread_cond_wait(&cond,&lock);
}
for(int i='A';i<='Z';i++)
printf("%c",i);
printf("\n");
pthread_mutex_unlock(&lock);
pthread_exit(0);
}
void main()
{
pthread_mutex_init(&lock,NULL);
pthread_cond_init(&cond,NULL);
pthread_t t1,t2;
pthread_create(&t1,NULL,thread1_write,NULL);
pthread_create(&t2,NULL,thread2_write,NULL);
pthread_join(t1,NULL);
pthread_join(t2,NULL);
pthread_cond_destroy(&cond);
pthread_mutex_destroy(&lock);
}
