#include<stdio.h>
#include<fcntl.h>
typedef struct result{
	int a;
	int b;
	char ch;
	int res;
}re;
int main(){
	int fd=open("/proc/proc_basic",O_RDWR);
	re r={.a=20,.b=10,.ch='+'},w;
	char buff[10];
	write(fd,&r,sizeof(re));
	printf("wrote %d, %d\n",r.a,r.b);
	read(fd,buff,sizeof(buff));
	printf("read %s\n",buff);
	close(fd);
}
