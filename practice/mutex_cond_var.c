#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

pthread_t t1,t2,t3;
pthread_mutex_t m1;
pthread_cond_t cvar;
int gvar=10,ready=0,done=0;
void* fun1(void *a)
{
	pthread_mutex_lock(&m1);
	gvar*=2;
	ready=1;
	printf("%d\n",gvar);
	pthread_mutex_unlock(&m1);
	pthread_cond_signal(&cvar);
	pthread_exit(NULL);
}
void* fun2(void *a)
{
	pthread_mutex_lock(&m1);
	while(!done){pthread_cond_wait(&cvar,&m1);}
	gvar*=gvar;
	printf("%d\n",gvar);
	pthread_mutex_unlock(&m1);
	return NULL;
}
void*fun3(void *b)
{

	pthread_mutex_lock(&m1);
	while(!ready){pthread_cond_wait(&cvar,&m1);}
	gvar=30;
	printf("%d\n",gvar);
	done=1;
	pthread_mutex_unlock(&m1);
	pthread_cond_signal(&cvar);
	return NULL;
}
int main()
{
	pthread_mutex_init(&m1,NULL);
	pthread_cond_init(&cvar,NULL);
	pthread_create(&t1,NULL,fun1,NULL);
	pthread_create(&t2,NULL,fun2,NULL);
	pthread_create(&t3,NULL,fun3,NULL);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	pthread_join(t3,NULL);
	pthread_mutex_destroy(&m1);
	pthread_cond_destroy(&cvar);
	return 0;
}
