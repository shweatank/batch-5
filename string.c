#include<stdio.h>
#include<string.h>
void main()
{
	char name[30];
	printf("Enter a name:");
	
	//scanf("%s",name);
	//scanf("%5s",name);
	gets(name);
	printf("%s",name);
	//printf("%.5s",name);
	//printf("%10.5s",name);
	//printf("%s",&name[2]);
	//puts(name);
}

