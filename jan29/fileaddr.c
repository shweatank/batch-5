#include<stdio.h>
#include<stdlib.h>

struct data
{
	char str[10];
	unsigned long *addr[4];
};

void delay(int dly)
{
	dly*=12000;
	while(dly--);
}

int main()
{
	char *p;
	struct data d;
	FILE *fp=fopen("file_address","r");
	if(fp==NULL)
	{
		perror("fopen");
		exit(0);
	}
	int i=0;
	while(fscanf(fp,"%s %p",d.str,&d.addr[i])!=EOF)
	{
		printf("%s %p\n",d.str,d.addr[i]);
		i++;
	}
	fclose(fp);
/*

	#define PINSEL0 d.addr[0]
	#define IODIR0  d.addr[1]
	#define IOSET0  d.addr[2]
	#define IOCLR0  d.addr[3]
	#define LED 1

	PINSEL0=0<<1;
	IODIR0=1<<1;
	while(1)
	{
		IOSET0=1<<LED;
		delay(10);
		IOCLR0=1<<LED;
	}*/
	return 0;
}
