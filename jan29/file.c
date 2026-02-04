#include<stdio.h>
#include<stdlib.h>

struct data
{
	char name[10];
	int age;
	char loc[10];
	char addr[10];
};

static int cnt;

int main()
{
	struct data d;
	char *fp=NULL;
	FILE *fd;
	while(1)
	{
		fd=fopen("data","r");
		if(!fd)
		{
			perror("fopen");
			exit(0);
		}
		while(fscanf(fd,"%s %d %s %s\n",d.name,&d.age,d.loc,d.addr)!=EOF)
		{
			printf("%s %d %s %s\n",d.name,d.age,d.loc,d.addr);	
		}
		fclose(fd);
	//	fseek(fd,0,SEEK_SET);
	}
	
}
