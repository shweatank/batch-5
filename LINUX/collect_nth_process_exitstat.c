#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<time.h>
int main(int argc,char*argv[])
{
	if(argc<3)
	{
		printf("a.out n_value which_child\n");
		return 0;
	}
	int n=atoi(argv[1]),k=atoi(argv[2]);
	int v=0,ret=0,status=0,arr[n],y=0;
	for(int i=0;i<n;i++)
	{
		ret=fork();
		if(ret==0)
		{
			srand(getpid());
			v=rand()%11+5;
			printf("child-%d with pid:%d ppid:%d sleeps for %d secs\n",i,getpid(),getppid(),v);
			sleep(v);
			exit(v);
		}
		else
		{
			arr[i]=ret;
	
		}
	}	
	waitpid(arr[k],&status,0);
	printf("parent waited for child with pid:%d which slept for:%d sec and with exit status:%d\n",arr[k],status>>8,status>>8);
}
