#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef struct st
{
	int roll;
	char name[20];
}ST;
int main()
{
	ST *p=NULL;
	p=(ST*)malloc(sizeof(ST));
	if(p==NULL)
	{
		printf("mem alloc failed\n");
		return 0;
	}
	p->roll=1;
	strcpy(p->name,"prasannna");
	printf("roll:%d  name:%s\n",p->roll,p->name);
	free(p);
	p=NULL;
	return 0;
}
