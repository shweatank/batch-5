#include<stdio.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<unistd.h>

#define CALC_IOC_MAGIC  'C'

#define CALC_IOC_ADD  _IOWR(CALC_IOC_MAGIC, 1, struct calc_req)
#define CALC_IOC_SUB  _IOWR(CALC_IOC_MAGIC, 2, struct calc_req)
#define CALC_IOC_MUL  _IOWR(CALC_IOC_MAGIC, 3, struct calc_req)
#define CALC_IOC_DIV  _IOWR(CALC_IOC_MAGIC, 4, struct calc_req)
#define CALC_IOC_MOD  _IOWR(CALC_IOC_MAGIC, 5, struct calc_req)
struct calc_req {
    int a;
    int b;
    long result;   // long to reduce overflow risk vs int (still possible)
    int err;       // 0 on success, -EINVAL/-ERANGE/-EDOM etc.
};

void main()
{
	struct calc_req cal;
	char op;
	
	int fd;
	fd=open("/dev/calci_ioctl",O_RDWR);
	if(fd<0)
	{
		perror("open");
		return;
	}
	printf("Enter the a value: \n");
	scanf("%d",&cal.a);
	printf("Enter the b value: \n");
	scanf("%d",&cal.b);
	printf("Enter the op: \n");
	scanf(" %c",&op);

	int ret;
	switch(op)
	{
		case '+':
		ret=ioctl(fd, CALC_IOC_ADD, &cal);
		break;
		case '-':
		ret=ioctl(fd, CALC_IOC_SUB, &cal);
		break;
		case '*':
		ret=ioctl(fd, CALC_IOC_MUL, &cal);
		break;
		case '/':
		ret=ioctl(fd, CALC_IOC_DIV, &cal);
		break;
		case '%':
		ret=ioctl(fd, CALC_IOC_MOD, &cal);
		break;
		default: 
		printf("invalid operator\n");
	}
	if(ret<0)
	{
		perror("ioctl");
		return;
	}

	if(cal.err)
	{
		printf("calculation error: %d\n",cal.err);
	}
	else
	{
		printf("Result=%ld\n",cal.result);
	}
	close(fd);
}


/*void main()
{
	struct calc_req cal;
	char op;
	
	int fd;
	fd=open("/dev/calci_ioctl",O_RDWR);
	if(fd<0)
	{
		perror("open");
		return;
	}
	printf("Enter the a value: \n");
	scanf("%d",&cal.a);
	printf("Enter the b value: \n");
	scanf("%d",&cal.b);
	printf("Enter the op: \n");
	scanf(" %c",&op);

	int ret;
	switch(op)
	{
		case '+':
		ret=ioctl(fd, CALC_IOC_ADD, &cal);
		break;
		case '-':
		ret=ioctl(fd, CALC_IOC_SUB, &cal);
		break;
		case '*':
		ret=ioctl(fd, CALC_IOC_MUL, &cal);
		break;
		case '/':
		ret=ioctl(fd, CALC_IOC_DIV, &cal);
		break;
		case '%':
		ret=ioctl(fd, CALC_IOC_MOD, &cal);
		break;
		default: 
		printf("invalid operator\n");
	}
	if(ret<0)
	{
		perror("ioctl");
		return;
	}

	if(cal.err)
	{
		printf("calculation error: %d\n",cal.err);
	}
	else
	{
		printf("Result=%ld\n",cal.result);
	}
	close(fd);
}*/
