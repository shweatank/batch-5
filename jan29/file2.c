#include<stdio.h>
#include<stdlib.h>

static int cnt;
struct data
{
	char name[10];
	int age;
	char location[10];
	char str[10];
};

int main()
{
	struct data d;
	FILE *fp=NULL;
	while(1)
	{
		fp=fopen("data","r");
		if(!fp)
		{
			perror("fopen");
			exit(0);
		}
		while(fscanf(fp,"%s %d %s %s",d.name,&d.age,d.location,d.str)!=EOF)
		{
			printf("%s %d %s %s \n",d.name,d.age,d.location,d.str);
		}
		fclose(fp);
	}
}
