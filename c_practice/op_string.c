#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>

int main()
{
	char str[10];
	int fd=open("/dev/string_driver",O_WRONLY);

	if(fd<0)
	{
		perror("open");
		return 0;
	}
	//infinite loop
	//for(int i=0;i<10;i++)
	{
		puts("num1,num2,operator");
		fgets(str,10,stdin);
		write(fd,str,sizeof(str));
		system("./string");
	}
	while(1);
	//read(fd,str,sizeof(str));
}
