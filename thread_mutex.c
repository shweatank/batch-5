#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

pthread_mutex_t lock;
pthread_cond_t cond;

 int flag=0;

void *print_1(void *p)
{
      //printf("lkjqLKNWDKNk\n");

        while(1)
        {
                pthread_mutex_lock(&lock);





                while(flag!=0)
                pthread_cond_wait(&cond,&lock);


             printf("%c\n",65);


                sleep(1);

                 flag=1;

                pthread_cond_signal(&cond);

                pthread_mutex_unlock(&lock);
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


           printf("%c\n",66);

            flag=0;
            sleep(1);



                 pthread_cond_signal(&cond);

                pthread_mutex_unlock(&lock);
        }
}
void main()
{
        pthread_t t1,t2;

     //printf("kjfksjbfkj\n");

        pthread_create(&t1,NULL,print_1,0);
        pthread_create(&t2,NULL,print_2,0);

        while(1);
}

