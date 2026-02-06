#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

struct st
{
        int res;
        char strop[10];
};
struct st var;
int main()
{
        int fd=open("/dev/task_calc",O_RDONLY);
        if(fd<0){perror("open");return 0;}
        if((read(fd,&var,sizeof(struct st)))<0)
                perror("read");
        printf("res=%d operation:%s\n",var.res,var.strop);
        return 0;
}

