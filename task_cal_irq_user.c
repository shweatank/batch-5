#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

struct st
{
	int a;
	int b;
	int res;
	int done;
	char strop[10];
};
int main()
{
	struct st in,out;
	in.a=5; in.b=10;
	int fd=open("/dev/task_calc",O_RDWR);
	write(fd,&in,sizeof(struct st));
	if((read(fd,&out,sizeof(struct st)))==0){perror("read");}
	printf("res=%d operation:%s\n",out.res,out.strop);
	return 0;
}
