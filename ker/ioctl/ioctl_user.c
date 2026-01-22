#include<stdio.h>
#include<sys/ioctl.h>
#include<sys/types.h>
#include<fcntl.h>
#define BASIC_IOCTL_MAGIC 'B'

struct result{
	int a;int b;int sum;int diff;int mul;int div;int mod;int err;
};

#define IOCTL_SET_VALUE _IOWR(BASIC_IOCTL_MAGIC, 1, struct result)

int main(){
	int fd=open("/dev/ioctl_malloc",O_RDWR);
	perror("open");
	struct result res={.a=89,.b=5};
	printf("sending to kernel %d, %d\n",res.a,res.b);
	printf("user IOCTL_SET_VALUE = 0x%x\n",IOCTL_SET_VALUE);
	int ret=ioctl(fd,IOCTL_SET_VALUE,&res);
	if(ret<0)
		perror("ioctl");
	printf("ioctl ret is %d\n",ret);
	printf("received from kernel %d, %d, %d, %d, %d\n",res.sum,res.diff,res.mul,res.div,res.mod);
}
