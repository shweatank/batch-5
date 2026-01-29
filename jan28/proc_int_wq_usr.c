#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>

struct d
{
	int n1,n2;
};
int result;
char ch;
struct d data;
int main()
{
	int fd=open("/proc/proc",O_RDWR);
	if(fd<0)
	{
		perror("open");
		exit(0);
	}
	data.n1=10;
	data.n2=20;
	write(fd,&data,sizeof(data));
	perror("write");
	while(1)
	{
		ch=getchar();
		if(ch=='A' || ch=='S' || ch=='M' || ch=='D')
		{
			int fd=open("/proc/proc",O_RDWR);
			read(fd,&result,sizeof(result));
			perror("read");
			printf("result : %d\n",result);
			break;
		}
	}
	return 0;
}
