#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
int main()
{
	char sub[10];
	mkfifo("hfd",0664);
	int fdr;
	fdr=open("hfd",O_RDONLY);
	while(1)
	{
	if((read(fdr,sub,sizeof(sub))) ==0)
	{
		perror("read");
		return 0;
	}
	puts("p2 read");
	puts(sub);
	}
}
