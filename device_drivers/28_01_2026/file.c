#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>

int main(){
	char s[10];
	int fd=open("/proc/proc_strrev",O_RDWR|0666);
	write(fd,"siva",4);
	read(fd,s,sizeof(s));
	printf("%s\n",s);
}

