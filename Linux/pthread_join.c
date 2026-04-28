#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
pthread_t t1,t2;
int var=12;
static u8 dec_to_bcd(int val)
{
    return ((val / 10) << 4) | (val % 10);
}
void *f1(void *p)
{
	int var1=8;
	printf("from f1\n");
}

void *f2(void *p)
{
}


int main()
{
	printf("hello from main..\n");
	pthread_create(&t1,NULL,f1,NULL);
	pthread_create(&t2,NULL,f2,NULL);

	
	
}
