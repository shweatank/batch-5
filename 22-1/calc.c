#include<stdio.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<unistd.h>

// ---------- IOCTL interface ----------
#define CALC_IOC_MAGIC  'C'

struct calc_req {
    int a;
    int b;
    long result;   // long to reduce overflow risk vs int (still possible)
    int err;       // 0 on success, -EINVAL/-ERANGE/-EDOM etc.
};


#define CALC_IOC_ADD  _IOWR(CALC_IOC_MAGIC, 1, struct calc_req)
#define CALC_IOC_SUB  _IOWR(CALC_IOC_MAGIC, 2, struct calc_req)
#define CALC_IOC_MUL  _IOWR(CALC_IOC_MAGIC, 3, struct calc_req)
#define CALC_IOC_DIV  _IOWR(CALC_IOC_MAGIC, 4, struct calc_req)
#define CALC_IOC_MOD  _IOWR(CALC_IOC_MAGIC, 5, struct calc_req)

struct calc_req v;
void main()
{
int fd=open("/dev/calc_ioctl",O_RDWR);;
if(fd<0)
{
perror("open");
return;
}
	v.a=20;
	v.b=10;
	char op;
	printf("enter the option {+,-,/,*,%}:\n");
	scanf("%c",&op);
	switch(op)
	{
		case '+':
			ioctl(fd,CALC_IOC_ADD,&v);
			printf("add=%ld\n",v.result);
			break;
		case '-':
			ioctl(fd,CALC_IOC_SUB,&v);
			printf("sub=%ld\n",v.result);
			break;
		case '*':
			ioctl(fd,CALC_IOC_MUL,&v);
			printf("MUL=%ld\n",v.result);
			break;
		case '/':
			ioctl(fd,CALC_IOC_DIV,&v);
			printf("div=%ld\n",v.result);
			break;
		case '%':
			ioctl(fd,CALC_IOC_MOD,&v);
			printf("mod=%ld\n",v.result);
			break;
		default:
			printf("invalid option\n");
	}
}
