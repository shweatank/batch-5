#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

#define DEV_NAME "/dev/my_ioctl"
struct st{
int a;
int b;
int res;
};

#define MAJIC 'a'
#define IOCTL_ADD _IOWR(MAJIC,1,struct st)
#define IOCTL_SUB _IOWR(MAJIC,2,struct st)
#define IOCTL_MUL _IOWR(MAJIC,3,struct st)
#define IOCTL_DIV _IOWR(MAJIC,4,struct st)
int op;
printf("Enter the option 1)add 2)sub 3)mul 4)div\n");
scanf("%d",&op);

switch(op){
case 1:ioctl(

