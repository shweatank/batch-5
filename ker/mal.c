#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main(){
	char *p=malloc(sizeof(char)*78);
	printf("address is %p\n",p);
	free(p);
}
