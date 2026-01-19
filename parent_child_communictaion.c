#include<stdio.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<unistd.h>
int v1,v2;
int a=5;
int main()
{
        int pfd[2],fd[2],s,d,s1,d1;
        if(((pipe(pfd))==-1)||((pipe(fd))==-1))
        {
                perror("pipe");
                return 0;
        }
        printf("enter data:");scanf("%d%d",&v1,&v2);
        int chpid=fork();
        if(chpid==-1)
        {
                perror("fork");
                return 0;
        }
        if(chpid>0)
        {
                a=7;
                printf("a=%d...address:%lu\n",a,&a);
                printf("parent..pid:%d,parent id:%d\n",getpid(),getppid());
                close(pfd[0]);
                close(fd[1]);
                s=v1+v2;
                write(pfd[1],&s,sizeof(int));
                read(fd[0],&d,sizeof(int));
                printf("difference:%d\n",d);
        }
        else
        {
                printf("child..pid:%d,parent pid:%d\n",getpid(),getppid());
                printf("a=%d...address:%lu\n",a,&a);
                close(pfd[1]);
                close(fd[0]);
                d1=v1-v2;
                write(fd[1],&d1,sizeof(int));
                read(pfd[0],&s1,sizeof(int));
                printf("sum:%d\n",s1);
        }
}

