#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
int i=0;
#define max 20

pthread_mutex_t lock1,lock2,lock3;

void *print_odd_numbers()
{
	while(i<=26)
	{

		pthread_mutex_lock(&lock1);
		if(i%2==0)
		{
//			sleep(1);
			printf("evne =%d\n",i);
			i++;
		}
		pthread_mutex_unlock(&lock2);
	}
}
void *print_even_numbers()
{
	 while(i<=26)
	 {

		pthread_mutex_lock(&lock2);
		 if(i%2!=0)
		 {
//			 sleep(1);
	          printf("odd = %d\n",i);
		 i++;
		 }
		pthread_mutex_unlock(&lock3);

	 }
}
void *print_alphabets()
{
       char ch='a';
	while(ch<='z')
	{
		 pthread_mutex_lock(&lock3);
//	         sleep(1);
	         printf("%c\n",ch++);
		pthread_mutex_unlock(&lock1);
	}
}

void main()
{
	pthread_t t1,t2,t3;

	pthread_mutex_lock(&lock2);
	pthread_mutex_lock(&lock3);

      pthread_create(&t1,NULL,print_even_numbers,0);
      pthread_create(&t2,NULL,print_odd_numbers,0);

      pthread_create(&t3,NULL,print_alphabets,0);

     // pthread_join(t1,0);
     // pthread_join(t2,0);

      pthread_exit(0);


      pthread_exit(NULL);
}


