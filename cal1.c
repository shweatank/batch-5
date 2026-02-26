#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
void main(int argc,char **argv)
{
	if(argc!=4)
	{
		printf("USAGE:./a.out n1 op n2\n");
		return;
	}
	struct st
	{
		int n1,n2;
		char ch;
	};
	int fp;
	int res;
	struct st v;
	v.n1=atoi(argv[1]);
	v.n2=atoi(argv[3]);
	v.ch=argv[2][0];
	fp=open("/dev/basic_char",O_RDWR|O_TRUNC|O_CREAT,0664);
	write(fp,&v,sizeof(struct st));
		
	read(fp,&res,sizeof(int));
	printf("result=%d\n",res);
//	printf("%d %d %c\n",v1.n1,v1.n2,v1.ch);

	close(fp);
}
