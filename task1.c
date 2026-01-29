#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<time.h>
void main(){
	FILE* fp=fopen("data.txt","r");
	char name[10],loc[20],prof[30];
	int age,flag=0;
while(1){	
	FILE* fp=fopen("data.txt","r");
fseek(fp,flag,SEEK_SET);
		if((fscanf(fp,"%s%d%s%s",name,&age,loc,prof))!=-1){
			printf("Name:%s	age:%d	location:%s	profession:%s\n",name,age,loc,prof);
		}
flag=ftell(fp);
fclose(fp);
sleep(1);
	}
	fclose(fp);

}
