#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/fcntl.h>

int main(int argc,char *argv[])
{
	int fd1,fd2,pos;
	if(argc==1)
	{
		printf("cp: missing file operand\nTry 'cp --help' for more information.\n");
		return 0;
	}
	if(argc==2)
	{
		printf("cp: missing destination file operand after 'Copy_Command.c'\nTry 'cp --help' for more information.\n");
		return 0;
	}
	if(argc>3)
	{
		printf("cp: target '%s': No such file or directory\n",argv[3]);
		return 0;
	}
	fd1=open(argv[1],O_RDONLY|O_EXCL);
	if(fd1==-1)
	{
		printf("%s is not found\n",argv[1]);
		return 0;
	}
	pos=lseek(fd1,SEEK_END,0);

}
