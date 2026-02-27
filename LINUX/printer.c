#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
int main()
{
	int v,fdr;
	mkfifo("prod",0666);
	fdr=open("prod",O_RDONLY);
	while((read(fdr,&v,sizeof(int)))>0)
	{
		printf("%d\n",v);
	}
	puts("squares witten succesfully\n");
}
