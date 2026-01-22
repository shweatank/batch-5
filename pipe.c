#include<stdio.h>
#include<unistd.h>
int main()
{
	int a=10,b=20;
	int fd[2],fd1[2];
	if(pipe(fd)==-1)
	{
		perror("pipe");
		return 0;
	}
	if(pipe(fd1)==-1)
	{
		perror("pipe");
		return 0;
	}

	int ret=fork();
	if(ret==-1)
	{
		perror("fork");
		return 0;
	}
	if(ret==0)
	{
		close(fd[0]);
		close(fd1[1]);
		//printf("enter the a and b\n");
		//scanf("%d %d",&a,&b);
		int res=a+b;
		int sub=0;

	//	printf("%d\n",res);
		write(fd[1],&res,4);
		read(fd1[0],&sub,4);
		printf("sub is %d\n",sub);
	
	}
	else
	{
		close(fd[1]);
		close(fd1[0]);
		int res1;
		read(fd[0],&res1,4);
		printf("add is %d\n",res1);
		int res2;
		res2=a-b;
		write(fd1[1],&res2,4);
	}
}





