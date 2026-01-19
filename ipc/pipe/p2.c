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
			int t;
			printf("in c %d %d\n",getpid(),getppid());
			read(pipefd[0],&t,sizeof(int));
			printf("t1=%d\n",t);
		}
			else
			{
				if(fork()==0)
				{
					int t;
                                  sleep(1);
					read(pipefd[0],&t,sizeof(int));
					printf("t2=%d\n",t);
				}
				else
				{
					int a[5]={10,2,4,12,5};
					printf("in parent %d\n",getpid());
					write(pipefd[1],a,sizeof(a));
					printf("write into pipe completed\n");
				}
			}
}
