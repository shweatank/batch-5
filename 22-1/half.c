#include"header.h"
int a[10];
int i=0;
pthread_mutex_t lock;
pthread_t t1,t2;
void*thread1_read(void*p)
{
pthread_mutex_lock(&lock);
printf( "read=%d\n",a[0]);
pthread_mutex_unlock(&lock);
}
void*thread1_write(void*p)
{
pthread_mutex_lock(&lock);
a[0]=10+i++;
printf("written\n");
pthread_mutex_unlock(&lock);
pthread_create(&t2,0,thread1_read,0);
pthread_join(t2,NULL);
}
void main()
{
printf("in main\n");
pthread_mutex_init(&lock,0);
pthread_create(&t1,0,thread1_write,0);
pthread_join(t1,NULL);
printf("after\n");
printf("after2\n");
//pthread_exit(0);
}
