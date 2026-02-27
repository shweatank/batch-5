#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
void sig_handler(int signum)
{
	printf("%d\n",signum);
	static char ch=65;
	printf("%c ",ch++);
}
int main()
{
	printf("going to handle SIGINT..in such a way that each encouter of sigint prints an alphabet\n");
	signal(SIGINT,sig_handler);
	while(1);
	return 0;
}
