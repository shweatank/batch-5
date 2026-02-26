#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void main()
{
	char s[]="30-15";
	FILE *fd=fopen("/dev/cal","w+");
	fprintf(fd,"%s",s);
	printf("Before:%s\n",s);
	fscanf(fd,"%s",s);
        printf("After:%s",s);
}

