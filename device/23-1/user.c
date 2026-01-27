#include<stdio.h>
#include<fcntl.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio_ext.h>
struct cal
{
	int a;
	int b;
	char ch;
	long res;
};
int main()
{
	int fd;
	fd=open("/dev/kbd_calc",O_RDWR);
	if(fd<0)
	{
		perror("open");
		return 0;
	}
	struct cal c;
	printf("enter the a and b\n");
	scanf("%d %d",&c.a,&c.b);
	printf("enter the operation\n");
	__fpurge(stdin);
	scanf("%c",&c.ch);
	write(fd,&c,sizeof(c));
}
