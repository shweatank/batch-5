#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
pthread_mutex_t lock;
pthread_cond_t con;
int flag=0;
void *thread1(void *p)
{
         char i='a';
	 while(i<='z')
	 {
		 pthread_mutex_lock(&lock);
		 while(flag!=0)
		 pthread_cond_wait(&con,&lock);
		 printf("%c ",i++);
		 flag=1;
		// fflush(stdout);
		 pthread_cond_signal(&con);
		 pthread_mutex_unlock(&lock);
	 }
}
void *thread2(void *p)
{
         char i='A';
	 while(i<='Z')
	 {
		 pthread_mutex_lock(&lock);
		 while(flag!=1)
		 pthread_cond_wait(&con,&lock);
		 printf("%c ",i++);
		 flag=0;
		 //fflush(stdout);
		 pthread_cond_signal(&con);
		 pthread_mutex_unlock(&lock);
	 }
}
int main()
{
	pthread_t t1,t2;
	pthread_create(&t1,NULL,thread1,0);
	pthread_create(&t2,NULL,thread2,0);
	pthread_join(t1,0);
	pthread_join(t2,0);
}
