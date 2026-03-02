#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
pthread_mutex_t lock;
pthread_cond_t cond;
int flag=0;
void *thread_1(void *p)
{
//	pthread_mutex_lock(&lock);
	 char i='a';
	while(i<='z')
	{

	      pthread_mutex_lock(&lock);

	      while(flag!=0)
		      pthread_cond_wait(&cond,&lock);

		printf("%c ",i++);
		flag=1;
		fflush(stdout);
		

		 pthread_cond_signal(&cond);
         	pthread_mutex_unlock(&lock);
	//	sleep(1);
	}
}
void *thread_2(void *p)
{
	 
//	pthread_mutex_lock(&lock);
	 char i='A';
	while(i<='Z')
	{

	     pthread_mutex_lock(&lock);
	     while(flag!=1)
		     pthread_cond_wait(&cond,&lock);
	 
		printf("%c ",i++);
		flag=0;
		fflush(stdout);

		pthread_cond_signal(&cond);

	     pthread_mutex_unlock(&lock);

	    // sleep(1);
	}
}
void main()
{
      pthread_t t1,t2;
      pthread_create(&t1,NULL,thread_1,0);
      pthread_create(&t2,NULL,thread_2,0);
      pthread_join(t1,NULL);
      pthread_join(t2,NULL);
}

