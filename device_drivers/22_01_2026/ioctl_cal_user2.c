#include<stdio.h>
#include<stdlib.h>
#include<linux/fs.h>
#include<fcntl.h>
#include <sys/ioctl.h>
#include<stdio_ext.h>

#define CALC_IOC_MAGIC  'C'
struct calc_req {
    int a;
    int b;
    long result;   // long to reduce overflow risk vs int (still possible)
    int err;       // 0 on success, -EINVAL/-ERANGE/-EDOM etc.
};

// Each ioctl copies struct calc_req in/out.
// User fills a,b ; driver fills result,err.

#define CALC_IOC_ADD  _IOWR(CALC_IOC_MAGIC, 1, struct calc_req)
#define CALC_IOC_SUB  _IOWR(CALC_IOC_MAGIC, 2, struct calc_req)
#define CALC_IOC_MUL  _IOWR(CALC_IOC_MAGIC, 3, struct calc_req)
#define CALC_IOC_DIV  _IOWR(CALC_IOC_MAGIC, 4, struct calc_req)
#define CALC_IOC_MOD  _IOWR(CALC_IOC_MAGIC, 5, struct calc_req)

int main(){
	int fd=open("/dev/calc_ioctl",O_RDWR);
	
	struct calc_req r;
	char ch;

	while(1){
		
		printf("+ for add\n- for sub\n/ for div\n* for mul\n%% for mod\ne for exit\n");
		printf("enter choice:");	
		scanf("%c",&ch);
		if(ch=='e')
			exit(0);
		printf("enter a:");
		scanf("%d",&r.a);
		printf("enter b:");
		scanf("%d",&r.b);
		__fpurge(stdin);

		switch(ch){
			case '+':
				ioctl(fd,CALC_IOC_ADD,&r);
				break;
			case '-':
				ioctl(fd,CALC_IOC_SUB,&r);
				break;
			case '*':
				ioctl(fd,CALC_IOC_MUL,&r);
				break;
			case '/':
				ioctl(fd,CALC_IOC_DIV,&r);
				break;
			case '%':
				ioctl(fd,CALC_IOC_MOD,&r);
				break;
		//	case 'e':
		//		exit(0);

		}
		printf("RESULT:%ld\n\n",r.result);
	}
}

