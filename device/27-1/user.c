#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
struct cal
{
	int a;
	int b;
	char ch;
	long res;
};
struct cal c;
int main()
{
	int fd;
	fd=open("/dev/kbd_calc",O_RDWR,0666);
	if(fd<1)
	{
		perror("open");
		return 0;
	}
	puts("enter the a and b");
	scanf("%d %d",&c.a,&c.b);
	write(fd,&c,sizeof(c));
		read(fd,&c,sizeof(c));
		switch(c.ch)
		{
			case'+':printf("res is %ld\n",c.res);
				break;
			case'-':printf("res is %ld\n",c.res);
				break;
			case'*':printf("res is %ld\n",c.res);
				break;
			case'/':printf("res is %ld\n",c.res);
				break;
		}
	
	close(fd);
}


