#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>

struct st{
	int a;
	int b;
	char ch;
	int res;
};

void main(){
	struct st v;
	v.a=100,v.b=200;
	int op;
	printf("Enter the operation 1)add 2)sub 3)mul 4)div\n");
	scanf("%d",&op);
	switch(op){
		case 1:v.ch='+';break;
		case 2:v.ch='-';break;
		case 3:v.ch='*';break;
		case 4:v.ch='/';break;
		default: printf("Invalid option\n");return;
	}

	int fd=open("/proc/proc_basic",O_WRONLY);
	write(fd,&v,sizeof(struct st));
	close(fd);
char res_buf[128];
	fd=open("proc/proc_basic",O_RDONLY);
	int n=read(fd,res_buf,sizeof(res_buf));
res_buf[n]='\0';
	printf("Result is %s\n",res_buf);
}

