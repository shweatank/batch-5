#include"header.h"
void main()
{
	if(fork()==0)
	{
		int i;
		//		sleep(1);
		printf(" in c1 befoe open fn..%d\n",getpid());
		int fd=open("f1",O_RDONLY);
		printf("in c1 after open fn\n");
		read(fd,&i,4);
		printf(" in c1 i=%d\n",i);
//while(1);
	}
	else
	{
		if(fork()==0)
		{
			int i;
			sleep(2);
			printf(" in c2 befoe open fn.. %d\n",getpid());
			int fd=open("f1",O_RDONLY);
			printf("in c2 after open fn\n");
			read(fd,&i,4);
			printf(" in c2 i=%d\n",i);
//while(1);
		}
		else
		{int i=20;
			printf("before par open function.. %d\n",getpid());
			int fd=open("f1",O_WRONLY);
			write(fd,&i,4);
//while(1);
		}
	}
}
