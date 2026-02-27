#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
int cnt=3;
void myhandler(int signum)
{
	printf("signum:%d ",signum);
	printf("SIGINT occured and handled\n");
	cnt--;
}
int main()
{
	struct sigaction var;
	var.sa_handler=myhandler;
	sigemptyset(&var.sa_mask);
	sigaddset(&var.sa_mask,SIGQUIT);
	sigprocmask(SIG_SETMASK, &var.sa_mask, NULL);
	var.sa_flags=0;
	sigaction(SIGINT,&var,NULL);
	while(cnt);
	var.sa_handler=SIG_IGN;
	sigaction(SIGINT,&var,NULL);
	while(1);
//	var.sa_handler=SIG_DFL;
//	sigaction(SIGINT,&var,NULL);
	//while(1);
}
