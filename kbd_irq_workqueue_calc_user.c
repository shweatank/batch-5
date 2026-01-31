#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
struct st
{
	int res;
	char strop[10];
};
int main()
{
	struct st var;
	int fd=open("/dev/kbd_irq_wq",O_RDONLY);
	if(fd<0){perror("open");return 0;}
	read(fd,&var,sizeof(struct st));
	perror("read");
	printf("res:%d strop:%s\n",var.res,var.strop);
	return 0;
}
