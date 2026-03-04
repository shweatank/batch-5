#include<stdio.h>
void isr()
{
}
void main()
{
	printf("hello world\n");

	int *p=malloc(4);

	fork();


	 *p=78;

	 sleep(1);

	// pause();

	 signal(2,isr);

	 printf("%d\n",*p);

	 printf("pid=%d\n",getpid());

	 int id=shmget(12,89,)

	 while(1);

}

