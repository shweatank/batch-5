#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdio_ext.h>

struct calci{
	char ch;
	int a;
	int b;
	int res;
};

int main(){
	int fd;
	struct calci c;


//	perror("open");

	while(1){
		fd=open("/proc/proc_kbd",O_RDWR|0666);
	/*	printf("a-add s-sub e-exit\n");
		__fpurge(stdin);
		printf("enter choice:");
		scanf("%c",&c.ch);
	
		if(c.ch=='e'){
			exit(0);
		}*/
	
		printf("enter a:");
		scanf("%d",&c.a);
		printf("enter b:");
		scanf("%d",&c.b);
		write(fd,&c,sizeof(c));
		read(fd,&c,sizeof(c));
		printf("RESULT of %d and %d is:%d\n",c.a,c.b,c.res);
		close(fd);
	}
}

