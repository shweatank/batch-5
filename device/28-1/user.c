#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdio_ext.h>
#include<stdio_ext.h>
struct cal
{
	int a;
	int b;
	long res;
	char ch;
};
int main()
{
	int fd;
	char ch;
	struct cal c;
	fd=open("/proc/proc_basic",O_RDWR);
	if(fd<0)
	{
		perror("open");
		return 0;
	}
	puts("enter the a and b");
	scanf("%d %d",&c.a,&c.b);
	write(fd,&c,sizeof(c));
	while(1)
	{
		__fpurge(stdin);
		ch=getchar();
		if(ch=='a'||ch=='s'||ch=='m')
		{
			fd=open("/proc/proc_basic",O_RDWR);
			read(fd,&c,sizeof(c));
			printf("%ld\n",c.res);
			close(fd);
		}
	}
}
