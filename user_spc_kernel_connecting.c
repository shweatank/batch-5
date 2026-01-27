#include<stdio.h>
#include<stdlib.h>
int main()
{
	FILE *fptr=fopen("/dev/basic_char","w+");
	fclose(fptr);
}
