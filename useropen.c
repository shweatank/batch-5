#include<stdio.h>
#include<stdlib.h>
void main()
{
	FILE *fd=fopen("/dev/basic_char","w");
	fclose(fd);

}
