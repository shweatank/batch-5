#include"header.h"

struct calc
{
	int a;
	int b;
	int res;
}v;
#define MAGIC 100

#define ADD _IOWR(MAGIC,1,struct calc)
#define SUB _IOWR(MAGIC,2,struct calc)
#define MUL _IOWR(MAGIC,3,struct calc)
#define DIV _IOWR(MAGIC,4,struct calc)
#define MOD _IOWR(MAGIC,5,struct calc)

void main()
{
	int fd=open("/dev/ioctl_driver",O_RDWR);
	if(fd<0)
	{
		perror("open");
		exit(0);
	}
	char op;
	printf("enter numbers two numbers\n");
	scanf("%d%d",&v.a,&v.b);
	printf("enter option\n");
	scanf(" %c",&op);
	switch(op)
	{
		case '+':
			ioctl(fd,ADD,&v);
			printf("add=%d\n",v.res);
			break;
		case '-':
			ioctl(fd,SUB,&v);
			printf("add=%d\n",v.res);
			break;
		case '*':
			ioctl(fd,MUL,&v);
			printf("add=%d\n",v.res);
			break;
		case '/':
			ioctl(fd,DIV,&v);
			printf("add=%d\n",v.res);
			break;
		case '%':
			ioctl(fd,MOD,&v);
			printf("add=%d\n",v.res);
			break;
		default:
			printf("wrong option\n");
	}
}
