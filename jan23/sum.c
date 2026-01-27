#include<stdio.h>

struct data
{
	int n1;
	int n2;
	int sum;
};

int main()
{
	struct data d;
	printf("enter num1: ");
	scanf("%d",&d.n1);
	printf("enter num2: ");
	scanf("%d",&d.n2);
	int fd=open("dev/keyboard_driver",O_RDWR);
	if(fd<0)
	{
		perror("open");
		exit(0);
	}

	write(fd,&d,sizeof(*d));
}
