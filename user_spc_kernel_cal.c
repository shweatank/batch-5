#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main(){
	char str[20]="10,20,-",*ptr=calloc(5,1);
	FILE *fptr=fopen("/dev/basic_char","w+");
	perror("fopen");
	fwrite(str,sizeof(char),strlen(str)+1,fptr);
	if((fread(ptr,sizeof(char),strlen(str),fptr))!=0)
	{
		printf("%s",ptr);
	}
	fclose(fptr);
}
