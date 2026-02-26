#include<stdio.h>
void main()
{
	while(1)
	{
	printf("apps calling a DD at the same time\n");
	system("ls");
	system("ls");
	system("cat system.c");
	}
}
