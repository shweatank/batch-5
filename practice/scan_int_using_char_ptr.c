#include<stdio.h>
#include<stdlib.h>
void checkprime(int  *a,int *b)
{
	int c=*a|*b,t=0;
	for(t=2;t<=c/2;t++)
	{
		if(c%t==0)
		{
			printf("%d not prime\n",c);
			return;
		}
	}
	printf("%d is a prime",c);
}
void data(char *p,int *num)
{
	for(int i=0;i<4;i++)
	{
		scanf("%d",p);
		printf("i:%d *p:%d\n",i,*p);
		p++;
	}
	
	printf("num:%d\n",*num);
}
int main()
{
	char *p;
	int n;
	p=&n;
	data(p,&n); //sending ptr...not its addr
	p=&n;
	int *f=malloc(sizeof(int)),*l=malloc(sizeof(int));
	*f=*p;p=p+3;
	*l=*p;
	checkprime(f,l);

}
