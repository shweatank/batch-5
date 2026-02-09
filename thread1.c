#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<fcntl.h>
#include<pthread.h>
int a=2;
int b=4;
pthread_mutex_t lock;
struct msgbuf
{
  long mtype;
  int data[4];
};
struct msgbuf v;   
void *add(void*p)
{
    pthread_mutex_lock(&lock);  
    printf("%d\n",a+b);
    pthread_mutex_unlock(&lock);
}
void *sub(void*p)
{
    pthread_mutex_lock(&lock);
    printf("%d\n",a-b);
    pthread_mutex_unlock(&lock);
}
void *mul(void*p)
{
   pthread_mutex_lock(&lock);
   printf("%d\n",a*b);
   pthread_mutex_unlock(&lock);
}    
int main()
{
   pthread_t t1,t2,t3;
   pthread_create(&t1,0,add,0);
   pthread_create(&t2,0,sub,0);
   pthread_create(&t3,0,mul,0);
   pthread_join(t1,NULL);//wait particular thread1 to terminate
   pthread_join(t2,NULL);   
   pthread_join(t3,NULL);
}   
