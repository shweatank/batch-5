
#include<stdio.h>
#include<unistd.h>
void main()
{
	int p[2];
	int q[2];
	pipe(p);
	pipe(q);
	printf("pipe1\nread=%d write=%d\n",p[0],p[1]);
	printf("pipe2\nread=%d write=%d\n",p[0],p[1]);
	if(fork())
	{
		int x=10,y=20,sum,div;
		sum=x+y;
		write(p[1],&sum,sizeof(int));
		read(q[0],&div,sizeof(int));
		printf("div:%d\n",div);


	}
	else
	{
		int sum,div,x=30,y=3;
		read(p[0],&sum,sizeof(int));
		printf("sum:%d\n",sum);
		div=x/y;
		write(q[1],&div,sizeof(int));
        }
}
