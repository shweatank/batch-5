#include"header.h"
int pipefd[2];
void main()
{
		if(pipe(pipefd)<0)
		{
			perror("pipe");
			return;
		}
	if(fork()==0)
	{
        sleep(1);
       int a[5],b[5];
	read(pipefd[0],a,sizeof(a));
	printf("%d %d %d %d %d\n",a[0],a[1],a[2],a[3],a[4]);\
	read(pipefd[0],b,sizeof(b));
        perror("read");
	printf("%d\n",b[0]);
	}
	else
	{
		int a[5]={10,2,4,12,5};
		printf("in parent %d\n",getpid());
		write(pipefd[1],a,sizeof(int)*5);
		printf("write into pipe completed\n");
	}
}
