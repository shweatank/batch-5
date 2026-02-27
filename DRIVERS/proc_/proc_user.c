#include<stdio.h>
#include<fcntl.h>

void main(){

int fd=open("/proc/proc_basic",O_RDONLY);
int res;
read(fd,&res,sizeof(int));
printf("Result=%d\n",res);
close(fd);
}
