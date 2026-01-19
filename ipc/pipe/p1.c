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
          int a[5],t;
         printf("in c %d %d\n",getpid(),getppid());
	read(pipefd[0],a,sizeof(int)*5);
        int i,j;
	for(i=0;i<4;i++)
	{
		for(j=i+1;j<5;j++)
		{
			if(a[j]<a[i])
			{
				t=a[i];
				a[i]=a[j];
				a[j]=t;
			}  
		}
	}
	for(i=0;i<5;i++)
	printf("%d ",a[i]);
	printf("\n");
	}
	else
	{
		int a[5]={10,2,4,12,5};
		printf("in parent %d\n",getpid());
		write(pipefd[1],a,sizeof(int)*5);
		printf("write into pipe completed\n");
	}
}
