#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include"common.h"

struct cal
{
	int a;
	int b;
	int c;
};

extern int flag;

struct cal v;

void main()
{

        flag=0;

          v.c=0;

	  struct cal v;
	   v.a=10;
	   v.b=20;

	  int fd= open("/dev/basic_char",O_WRONLY);
	  perror("open");

	  write(fd,&v,sizeof(v));
	  perror("write");

	  //while(flag==0);

	  read(fd,&v,sizeof(v));
	  perror("read");

	  printf("%d\n",v.c);

	  close(fd);
}
