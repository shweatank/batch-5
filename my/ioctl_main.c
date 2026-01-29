#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/ioctl.h>

#define DEV_NAME "my_ioctl"
struct st{
	int a;
	int b;
	int res;
};

#define MAJIC 'B'
#define IOCTL_ADD _IOWR(MAJIC,1,struct st)
#define IOCTL_SUB _IOWR(MAJIC,2,struct st)
#define IOCTL_MUL _IOWR(MAJIC,3,struct st)
#define IOCTL_DIV _IOWR(MAJIC,4,struct st)
void main(){
	int op;
	int fp=open(DEV_NAME,O_RDONLY);
	printf("Enter the option 1)add 2)sub 3)mul 4)div\n");
	scanf("%d",&op);
	struct st v;
v.a=100,v.b=200;
	switch(op){
		case 1:ioctl(fp,IOCTL_ADD,&v);break;
		case 2:ioctl(fp,IOCTL_SUB,&v);break;
		case 3:ioctl(fp,IOCTL_MUL,&v);break;
		case 4:ioctl(fp,IOCTL_DIV,&v);break;
		default:printf("Invalid option\n");return;
	}

	printf("result=%d\n",v.res);
	close(fp);
}


