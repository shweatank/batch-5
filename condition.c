#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

pthread_mutex_t lock;
pthread_cond_t cond;

 int flag=0;

 char ch='a';
 char ch2='A';
 int num=1;

void *print_1(void *p)
{
      //printf("lkjqLKNWDKNk\n");
      //

	 while(1)
	 {

 
                pthread_mutex_lock(&lock);

                while(flag!=0)
                pthread_cond_wait(&cond,&lock);


             printf("%c\n",ch++);


               // sleep(1);

                 flag=1;

                //pthread_cond_signal(&cond);
                pthread_cond_broadcast(&cond);
                pthread_mutex_unlock(&lock);

		//sleep(1);
	 }
       
}

void *print_2(void *p)
{
     //printf("kjfksjbfkj\n");

        while(1)
        {

                pthread_mutex_lock(&lock);

             while(flag!=1)
            pthread_cond_wait(&cond,&lock);


           printf("%c\n",ch2++);

            flag=2;
           // sleep(1);

               //  pthread_cond_signal(&cond);

	        pthread_cond_broadcast(&cond);
                pthread_mutex_unlock(&lock);


            //sleep(1);
        }
}

void *print_3(void *p)
{
	while(1)
	{
		pthread_mutex_lock(&lock);
		
			while(flag!=2)
				pthread_cond_wait(&cond,&lock);

		printf("%d\n",num++);

		flag=0;

	//	pthread_cond_signal(&cond);
         	pthread_cond_broadcast(&cond);

		pthread_mutex_unlock(&lock);

		//sleep(1);
	}
}
void main()
{
        pthread_t t1,t2,t3;

     //printf("kjfksjbfkj\n");

        pthread_create(&t1,NULL,print_1,0);
        pthread_create(&t2,NULL,print_2,0);
        pthread_create(&t3,NULL,print_3,0);

        while(1);
}
