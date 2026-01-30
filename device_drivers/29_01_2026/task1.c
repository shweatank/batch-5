#include<stdio.h>

struct st{
	char name[100];
	int age;
	char loc[100];
	char prof[100];
};

int main(){
	struct st s;
	FILE *fp=fopen("file7","r");
	while(1){
		while(fscanf(fp,"%s %d %s %s",s.name,&s.age,s.loc,s.prof)==4){
			printf("New Entry\n");
			printf("Name:%s\n",s.name);
			printf("Age:%d\n",s.age);
			printf("Name:%s\n",s.loc);
			printf("Name:%s\n",s.prof);
			printf("\n");
		}
	}	
	fclose(fp);
}

