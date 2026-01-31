#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
struct st
{
	int a;
	int b;
	int rout;
	char op;
};
int main()
{
	struct st var={.a=10,.b=20,.rout=0,.op='m'};
	struct st out;
	int fd=open("/proc/proc_calc",O_RDWR);
	if(fd<0)
	{
		perror("open");
		return 0;
	}
	write(fd,&var,sizeof(struct st));
	if((read(fd,&out,sizeof(struct st)))==0){
		perror("read");}
	printf("res:%d\n",out.rout);
}

