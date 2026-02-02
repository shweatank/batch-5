#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
void delayus(unsigned int delay)
{
	delay*=12;
	while(delay--);
}
int main()
{
	FILE* fd;
	fd=fopen("adress.txt","r");
	if(fd==NULL)
	{
		perror("fopen");
		return 0;
	}
	int x,add[4],i=0;
	while(fscanf(fd,"%x",add+i)!=EOF)
	{
		printf("%x ",add[i]);
		i++;
	}
	unsigned int IOPIN0=add[0];
	unsigned int IOSET0=add[1];
	unsigned int IODIR0=add[2];
	unsigned int IOCLR0=add[3];
	IODIR0=1<<7;
	IOSET0=1<<7;
	delayus(200);
	IOCLR0=1<<7;
	delayus(200);


}
