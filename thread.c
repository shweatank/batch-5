#include<stdio.h>
#include"header.h"
pthread_mutex_t lock;
int j=20;
void *thread1(void*arg)
{
  pthread_mutex_lock(&lock);
  j++;
  printf("%d\n",j);
  printf("%s\n",(char*)arg);
  pthread_mutex_unlock(&lock);
}
void *thread2(void*arg1)
{
  pthread_mutex_lock(&lock);
  j++;
  printf("%d\n",j);
  printf("%d\n",(int*)arg1); 
  pthread_mutex_unlock(&lock);
}
int main()
{
 int i=10;
 pthread_t t1,t2;
 pthread_create(&t1,NULL,thread1,"hello");
 pthread_create(&t2,NULL,thread2,&i);
 pthread_join(t1,NULL);
 pthread_join(t2,NULL);
// pthread_destroy(t1);
// pthread_destroy(t2);
}
   
