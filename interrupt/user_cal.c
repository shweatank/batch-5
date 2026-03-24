#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

struct val
{
	int a,b;
};

void main(int argc,char **argv)
{
	if(argc!=3)
	{
		printf("USAGE:./a.out n1 n2\n");
		return;
	}
	struct val v;
	int fp;
	v.a=atoi(argv[1]);
	v.b=atoi(argv[2]);
	fp=open("/dev/basic_char",O_WRONLY|O_TRUNC|O_CREAT,0664);
	perror("open");
	write(fp,&v,sizeof(struct val));
	perror("write");
 //       read(fp,&res,sizeof(int));
   //     printf("result=%d\n",res);
//      printf("%d %d %c\n",v1.n1,v1.n2,v1.ch);

        close(fp);
	perror("close");
}
