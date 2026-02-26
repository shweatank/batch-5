#include<stdio.h>
void main()
{
	int age;
	char name[50],location[50],profession[50];
	FILE *fd=fopen("data","r");
while(1)	
{
	while(fscanf(fd,"%s %d %s %s",name,&age,location,profession)!=4)
	{
	printf("Name:%s  Age:%d  Location:%s  profession:%s\n",name,age,location,profession);
	}
}
	fclose(fd);

}

