#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/types.h>
#include<stdio.h>
struct mymsg
{
	long mtype;
	int data;
}v;
