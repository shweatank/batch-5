// basic_ioctl_user.c
// Minimal user program calling ioctl

#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include<stdlib.h>
#include<string.h>

#define CALC_IOC_MAGIC 'C'
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

int main(int argc,char **argv)
{
	if(argc!=4)
	{
		printf("USAGE:./a.out n1 n2 op\n");
		return 0;
	}

    int fd;
    int cmd;
//    char cmd[20];
    struct calc_req v;
    v.a=atoi(argv[1]);
    v.b=atoi(argv[2]);
    v.result=0;
    v.err=0;
    
    
    
    fd = open("/dev/basic_ioctl", O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    printf("User: sending %d %d %c %ld to kernel\n",v.a,v.b,argv[3][0],v.result);
    

    
    if(argv[3][0]=='+')
    {
    if(ioctl(fd,CALC_IOC_ADD, &v)<0)
    {
	    perror("ioctl");
	    return 0;
    }
    }
//	cmd=1;//    strcpy(cmd,"CALC_IOC_ADD");
    else if(argv[3][0]=='-')
    {
	    if(ioctl(fd,CALC_IOC_SUB, &v)<0)
    {
	    perror("ioctl");
	    return 0;
    }}
//	cmd=2; //    strcpy(cmd,"CALC_IOC_SUB");
    else if(argv[3][0]=='*')
    {if(ioctl(fd,CALC_IOC_MUL, &v)<0)
    {
	    perror("ioctl");
	    return 0;
    }}
//	  cmd=3;//  strcpy(cmd,"CALC_IOC_MUL");
    else if(argv[3][0]=='/')
    {if(ioctl(fd,CALC_IOC_DIV, &v)<0)
    {
	    perror("ioctl");
	    return 0;
    }}
//	  cmd=4; //  strcpy(cmd,"CALC_IOC_DIV");
    else if(argv[3][0]=='%')
    {if(ioctl(fd,CALC_IOC_MOD, &v)<0)
    {
	    perror("ioctl");
	    return 0;
    }}
//	   cmd=5; // strcpy(cmd,"CALC_IOC_MOD");
    else
    {	    printf("incorrect operator\n");
		return 0;
    }


	  printf("User: got back %d %d %c %ld from kernel\n",v.a,v.b,argv[3][0],v.result);

    close(fd);
    return 0;
}
