#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main()
{
	int ret=fork();
	if(ret>0)
	{
	while(1)
	{
	system("./user");
	}
	}
	else
	{
	while(1)
        {
        system("./exe");
        }
	}

}
