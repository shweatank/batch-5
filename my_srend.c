#include<stdio.h>
int my_srend(char *p,char *q)
{
	char *res=q;

	while(*p) p++;

	while(*q) q++;

	while(*p == *q && q>res)
	{
		p--;q--;
	}

	if(*p==*q)
		return 1;
	else
		return 0;
}

int main()
{

	char p[]="gurunth",q[]="nath";
  
	 my_srend(p,q)?printf("yes it is occured:\n"):printf("no it is not there:\n");
}
