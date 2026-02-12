#include<stdio.h>
#include<stdlib.h>

int main(){
	FILE* fp=fopen("file","r");	
	
	if(fp==NULL){
		printf("error\n");
		exit(0);
	}
	printf("%p ",fp);
	printf("%c\n",*fp);
	//fp++;
	printf("%p ",fp+1);
	printf("%c\n",*(char*)fp+1);
}

