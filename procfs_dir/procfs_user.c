#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

struct op
{
	   char op;
	   int a;
	   int b;
	   int res;
};
int main()
{
	int fd=open("/proc/proc_basic",	O_RDWR);
	if(fd<0)
	{
	perror("open");
	return 0;
	}


	 struct op result;
	struct op v={'+',100,200,0};

	write(fd,&v,sizeof(v));
	perror("write");


	read(fd,&result,sizeof(result));
	perror("read");

	printf("result=%d\n",result.res);


	
}
