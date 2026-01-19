#include"header.h"
int sum,diff,mul,div1,n1=60,n2=20;
void* add(void*p)
{
sum=n1+n2;
pthread_exit(&sum);
}
void *sub(void*p)
{
diff=n1-n2;
pthread_exit(&sub);
}
void *multiply(void*p)
{
mul=n1*n2;
pthread_exit(&mul);
}
void *division(void*p)
{
div1=n1/n2;
pthread_exit(&div1);
}
void main()
{
int *res;
pthread_t t1,t2,t3,t4;
pthread_create(&t1,0,add,0);
pthread_create(&t2,0,sub,0);
pthread_create(&t3,0,multiply,0);
pthread_create(&t4,0,division,0);
pthread_join(t1,NULL);
printf("add=%d\n",sum);
pthread_join(t2,NULL);
printf("sub=%d\n",diff);
pthread_join(t3,NULL);
printf("mul=%d\n",mul);
pthread_join(t4,NULL);
printf("div=%d\n",div1);
}
