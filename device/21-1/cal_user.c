#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<stdio_ext.h>
#include<fcntl.h>
#include<linux/fs.h>
#include<unistd.h>
#include<sys/ioctl.h>

struct calc_req {
	int a;
	int b;
	long result;   // long to reduce overflow risk vs int (still possible)
	int err;       // 0 on success, -EINVAL/-ERANGE/-EDOM etc.
};

#define CALC_IOC_MAGIC 'C'

#define CALC_IOC_ADD  _IOWR(CALC_IOC_MAGIC, 1, struct calc_req)
#define CALC_IOC_SUB  _IOWR(CALC_IOC_MAGIC, 2, struct calc_req)
#define CALC_IOC_MUL  _IOWR(CALC_IOC_MAGIC, 3, struct calc_req)
#define CALC_IOC_DIV  _IOWR(CALC_IOC_MAGIC, 4, struct calc_req)
#define CALC_IOC_MOD  _IOWR(CALC_IOC_MAGIC, 5, struct calc_req)




int main()
{
	int fd;
	struct calc_req a;
	fd=open("/dev/calc",O_RDWR);
	if(fd<0)
	{
		perror("open");
		return 0;
	}
	char ch;
	while(1)
	{
		puts("enter the a nd b");
		scanf("%d %d",&a.a,&a.b);
		__fpurge(stdin);
		puts("enter the operation");
		scanf("%c",&ch);
		switch(ch)
		{
			case'+':ioctl(fd,CALC_IOC_ADD,&a);
				break;
			case'-':ioctl(fd,CALC_IOC_SUB,&a);
				break;
			case'*':ioctl(fd,CALC_IOC_MUL,&a);
				break;
			case'/':ioctl(fd,CALC_IOC_DIV,&a);
				break;
			case'%':ioctl(fd,CALC_IOC_MOD,&a);
				break;
			case'e':exit(0);
		}
		printf("calculated value is %ld\n",a.result);
	}
	close(fd);
}
