#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include"common.h"
void main()
{
	int fd;
	struct data d;
	struct result r;

	printf("Enter a value: ");
	scanf("%d", &d.a);

	printf("Enter b value: ");
	scanf("%d", &d.b);

	printf("Enter operator (+ - * /): ");
	scanf(" %c", &d.op);  

	fd = open("/dev/basic_char", O_RDWR);
	if (fd < 0) {
		perror("open");
		return;
	}

	write(fd, &d, sizeof(d));
	lseek(fd, 0, SEEK_SET);

	read(fd, &r, sizeof(r));

	printf("Result = %d\n", r.res);



	/*char write_buf[100]="hello";
	char read_buf[100];

	int fd=open("/dev/basic_char",O_RDWR);
	if(fd<0)
	{
		perror("open");
		return;
	}
	write(fd,write_buf,strlen(write_buf));
	lseek(fd,0,SEEK_SET);

	read(fd,read_buf,sizeof(read_buf));
	printf("User receieved: %s\n",read_buf);*/
	
	close(fd);
}
