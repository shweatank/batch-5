#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int main()
{
	char str[20]="hello",*ptr=calloc(1,1);;
	FILE*fptr=fopen("dum.txt","w+");
	if((fwrite(str,sizeof(char),strlen(str),fptr))==0)
	{
		perror("fwrite");return 0;
	}
	while((fread(ptr,sizeof(char),1,fptr))!=0)
	{
		printf("%c",*ptr);
	}
	//printf("\n");
	fclose(fptr);
}
