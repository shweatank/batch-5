#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define DEVICE "/dev/basic_ioctl"
#define IOCTL_MAGIC 'B'

struct st {
	int a;
	int b;
	int res;
};

#define IOCTL_ADD _IOWR(IOCTL_MAGIC, 1, struct st)
#define IOCTL_SUB _IOWR(IOCTL_MAGIC, 2, struct st)
#define IOCTL_MUL _IOWR(IOCTL_MAGIC, 3, struct st)
#define IOCTL_DIV _IOWR(IOCTL_MAGIC, 4, struct st)

void main()
{
	int fd;
	struct st v;
	v.a = 10;
	v.b = 20;
	fd = open(DEVICE, O_RDWR);
	perror("open");
	printf("User:a=%d,b=%d\n",v.a,v.b);
	int op;
	printf("Enter the operation:1)add 2)sub 3)mul 4)divi\n");
	scanf("%d",&op);
	switch(op){
		case 1:ioctl(fd,IOCTL_ADD,&v);break;
		case 2:ioctl(fd,IOCTL_SUB,&v);break;
		case 3:ioctl(fd,IOCTL_MUL,&v);break;
		case 4:ioctl(fd,IOCTL_DIV,&v);break;
		default: printf("Invalid option entered\n");return ;
	}
	printf("User: result = %d\n", v.res);
	close(fd);
}

